#include "rocket/common/log.h"
#include "rocket/net/tcp/net_addr.h"


int main() {
    rocket::Config::SetGlobalConfig("/home/anyz/projects/rocket/conf/rocket.xml");
    rocket::Logger::InitGlobalLogger();

    rocket::IPNetAddr addr("127.0.0.1", 12345);
    DEBUGLOG("create addr %s", addr.toString().c_str());
    return 0;
}