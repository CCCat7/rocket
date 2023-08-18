#ifndef TINYRPC_NET_HTTP_HTTP_DISPATCHER_H
#define TINYRPC_NET_HTTP_HTTP_DISPATCHER_H

#include <map>
#include <memory>
#include "rocket/net/http/abstract_dispatcher.h"
#include "rocket/net/http/http_servlet.h"


namespace rocket {

class HttpDispacther : public AbstractDispatcher {
 public:
 typedef std::shared_ptr<HttpDispacther> ptr;
  static HttpDispacther::ptr GetHttpDispatcher();

  HttpDispacther() = default;

  ~HttpDispacther() = default;

  void dispatch(AbstractData* data, TcpConnection* conn);

  void registerServlet(const std::string& path, HttpServlet::ptr servlet);

 public:
  std::map<std::string, HttpServlet::ptr> m_servlets;
};


}



#endif