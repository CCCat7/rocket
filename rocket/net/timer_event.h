#ifndef ROCKET_NET_TIMER_EVENT_H
#define ROCKET_NET_TIMER_EVENT_H

#include <functional>
#include <memory>

namespace rocket {

class TimerEvent {
public:
    typedef std::shared_ptr<TimerEvent> s_ptr;

    TimerEvent(int interval, bool is_repeated, std::function<void()> cb);

    int64_t getArriveTime() const {
        return m_arrive_time;
    } 

    void setCanceler(bool value) {
        m_is_canceled = value;
    }

    bool isCanceled() {
        return m_is_canceled;
    }

    bool isRepeated() {
        return m_is_repeated;
    }

    std::function<void()> getCallBack() {
        return m_task;
    }

    void resetArriveTime();

private:
    int64_t m_arrive_time;
    int64_t m_interval;

    bool m_is_repeated {false};
    bool m_is_canceled {false};

    std::function<void()> m_task;
};

}

#endif