#ifndef ROCKET_COMMON_LOG_H
#define ROCKET_COMMON_LOG_H

#include <string>

namespace rocket {

enum logLevel {
    Debug = 1,
    Info = 2,
    Error = 3
};

class logEvent
{
private:
    std::string m_file_name;  // file name
    std::string m_file_line;  // line no.
    int32_t m_pid;
    int32_t m_thread_id;
    std::string m_time;
public:
    logEvent(/* args */);
    ~logEvent();
    std::string getFileName() { return m_file_name; }
    std::string getFileLine() { return m_file_line; }
};

logEvent::logEvent(/* args */)
{
}

logEvent::~logEvent()
{
}


}

#endif