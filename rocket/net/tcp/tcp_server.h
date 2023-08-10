#ifndef ROCKET_NET_TCP_TCP_SERVER_H
#define ROCKET_NET_TCP_TCP_SERVER_H

#include <set>

#include "rocket/net/tcp/tcp_acceptor.h"
#include "rocket/net/tcp/net_addr.h"
#include "rocket/net/eventloop.h"
#include "rocket/net/io_thread_group.h"

namespace rocket {

class TcpServer {
public:
    TcpServer(NetAddr::s_ptr local_addr);
    ~TcpServer();

    void start();

private:
    void init();

    void onAccept();

private:
    TcpAcceptor::s_ptr m_acceptor;
    NetAddr::s_ptr m_local_addr;

    EventLoop *m_main_event_loop {nullptr}; // main reactor

    IOThreadGroup *m_io_thread_group {nullptr};  // subReactor group

    FdEvent *m_listen_fd_event;

    int m_client_counts {0};

    std::set<TcpConnection::s_ptr> m_client;
};

} // namespace rocket


#endif