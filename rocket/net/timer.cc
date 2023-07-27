#include <sys/timerfd.h>
#include <string.h>
#include "rocket/net/timer.h"
#include "rocket/common/log.h"
#include "rocket/common/util.h"

namespace rocket
{

Timer::Timer() : FdEvent() {
    m_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    DEBUGLOG("timer fd=%d", m_fd);

    //fd 可读事件放到eventloop上监听
    listen(FdEvent::IN_EVENT, std::bind(&Timer::onTimer, this));
}

Timer::~Timer() {

}

void Timer::onTimer() {
    //处理缓冲区数据，防止下一次继续触发可读事件
    char buf[8];
    while (1) {
        if (read(m_fd, buf, 8) && errno == EAGAIN) {
            break;
        }
    }

    //执行定时任务
    ino64_t now = getNowMs();

    std::vector<TimerEvent::s_ptr> tmps;
    std::vector<std::pair<ino64_t, std::function<void()>>> tasks;

    ScopeMutex<Mutex> lock(m_mutex);
    auto it = m_pending_events.begin();

    for (it = m_pending_events.begin(); it != m_pending_events.end(); ++it) {
        if ((*it).first <= now) {
            if (!(*it).second->isCanceled()) {
                tmps.push_back((*it).second);
                tasks.push_back(std::make_pair((*it).second->getArriveTime(), (*it).second->getCallBack()));
            }
            
        }
        else {
            break;
        }
    }

    m_pending_events.erase(m_pending_events.begin(), it);
    lock.unlock();

    //需要把重复的event再次添加进去
    for (auto i = tmps.begin(); i != tmps.end(); ++i) {
        if ((*i)->isRepeated()) {
            //调整arrivetime
            (*i)->resetArriveTime();
            addTimerEvent(*i);
        }
    }

    resetArriveTime();

    for (auto i: tasks) {
        if (i.second) {
            i.second();
        }
    }
}

void Timer::resetArriveTime() {
    ScopeMutex<Mutex> lock(m_mutex);
    auto tmp = m_pending_events;
    lock.unlock();

    if (!tmp.size()) {
        return;
    }

    int64_t now = getNowMs();

    auto it = tmp.begin();
    int64_t interval = 0;
    if (it->second->getArriveTime() > now) {
        interval = it->second->getArriveTime();
    }
    else {
        interval = 100;
    }

    timespec ts;
    ts.tv_sec = interval / 1000;
    ts.tv_nsec = (interval % 1000) * 1000000;

    itimerspec value;
    value.it_value = ts;
    int rt = timerfd_settime(m_fd, 0, &value, NULL);
    if (rt != 0) {
        ERRORLOG("timerfd_settime error, errno=%d, error=%s", errno, strerror(errno))
    }
}

void Timer::addTimerEvent(TimerEvent::s_ptr event) {
    bool is_reset_timerfd = false;

    ScopeMutex<Mutex> lock(m_mutex);
    if (m_pending_events.empty()) {
        is_reset_timerfd = true;
    }
    else {
        auto it = m_pending_events.begin();
        if ((*it).second->getArriveTime() > event->getArriveTime()) {
            is_reset_timerfd = true;
        }
    }
    m_pending_events.emplace(event->getArriveTime(), event);
    lock.unlock();

    if (is_reset_timerfd) {
        resetArriveTime();
    }
}

void Timer::deleteTimerEvent(TimerEvent::s_ptr event) {
    event->setCanceler(true);

    ScopeMutex<Mutex> lock(m_mutex);
    
    auto begin = m_pending_events.lower_bound(event->getArriveTime());
    auto end = m_pending_events.upper_bound(event->getArriveTime());

    auto it = begin;
    for (it = begin; it != end; ++it) {
        if (it->second == event) {
            break;
        }
    }

    if (it != end) {
        m_pending_events.erase(it);
    }
    lock.unlock();
    DEBUGLOG("success delete TimerEvent at arrive time &lld", event->getArriveTime());
}


} // namespace rocket
