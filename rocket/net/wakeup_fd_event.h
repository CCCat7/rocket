#ifndef ROCKET_NET_WAKEUP_FD_EVENT_H
#define ROCKET_NET_WAKEUP_FD_EVENT_H

#include "rocket/net/fd_event.h"

namespace rocket {

class WakeupFdEvent : public FdEvent {
public:
    WakeupFdEvent(int fd);
    ~WakeupFdEvent();

    void wakeup();
private:
    

};




} // namespace rocket


#endif