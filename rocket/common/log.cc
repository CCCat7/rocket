#include <sys/time.h>
#include <sstream>
#include "rocket/common/log.h"
#include "rocket/common/util.h"
#include "rocket/common/config.h"


namespace rocket {

static Logger *g_logger = nullptr;

std::string LogLevelToString(LogLevel level) {
    switch (level)
    {
    case Debug:
        return "Debug";
    case Info:
        return "Info";
    case Error:
        return "Error";
    default:
        return "UNKONWN";
    }
}

LogLevel StringToLogLevel(const std::string &log_level) {
    if (log_level == "Debug") {
        return Debug;
    }
    else if (log_level == "Info") {
        return Info;
    }
    else if (log_level == "Error") {
        return Error;
    }
    else {
        return Unknown;
    }
}

std::string LogEvent::toString() {
    struct timeval now_time;
    gettimeofday(&now_time, nullptr);

    struct tm now_time_t;
    localtime_r(&(now_time.tv_sec), &now_time_t);
    char buf[128];
    strftime(&buf[0], 128, "%y-%m-%d %H:%M:%S", &now_time_t);
    std::string time_str(buf);

    int ms = now_time.tv_usec / 1000;
    time_str = time_str + "." + std::to_string(ms);

    m_pid = getPid();
    m_thread_id = getThreadId();

    std::stringstream ss;

    ss << "[" << LogLevelToString(m_level) << "]\t"
        << "[" << time_str << "]\t"
        << "[" << m_pid << ":" << m_thread_id << "]\t"
        << "[" << std::string(__FILE__) << ":" << __LINE__ << "]\t";

    return ss.str();
}

void Logger::log() {
    while (!m_buffer.empty()) {
        std::string msg = m_buffer.front();
        m_buffer.pop();
        printf(msg.c_str());
    }

    
}

Logger* Logger::GetGlobalLogger() {
    return g_logger;
}

void Logger::InitGlobalLogger() {
    LogLevel global_log_level = StringToLogLevel(Config::GetGlobalConfig()->m_log_level);
    printf("Init log level [%s]", LogLevelToString(global_log_level).c_str());
    g_logger = new Logger(global_log_level); 
}

void Logger::pushLog(const std::string &msg) {
    m_buffer.push(msg);
}



}