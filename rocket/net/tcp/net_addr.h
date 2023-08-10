#ifndef ROCKET_NET_TCP_NET_ADDR_H
#define ROCKET_NET_TCP_NET_ADDR_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

namespace rocket {

class NetAddr {
public:
    virtual sockaddr* getSockAddr() = 0;

    virtual socklen_t getSockLen() = 0;

    virtual int getFamily() = 0;

    virtual std::string toString() = 0;

};

class IPNetAddr : public NetAddr {

public:
    IPNetAddr(const std::string &ip, uint16_t port);
    IPNetAddr(const std::string &addr);
    IPNetAddr(sockaddr_in addr);

    sockaddr* getSockAddr();

    socklen_t getSockLen();

    int getFamily();

    std::string toString();

private:
    std::string m_ip;
    uint16_t m_port {0};

    sockaddr_in m_addr;
};


} // namespace rocket


#endif