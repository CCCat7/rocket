#include <google/protobuf/service.h>
#include <memory>
#include "rocket/net/http/http_dispatcher.h"
#include "rocket/net/http/http_request.h"
#include "rocket/net/http/http_servlet.h"


namespace rocket {

//for rpc
static HttpDispacther* g_http_dispatcher = NULL;

HttpDispacther* HttpDispacther::GetHttpDispatcher() {
    if (g_http_dispatcher != NULL) {
        return g_http_dispatcher;
    }
    g_http_dispatcher = new HttpDispacther;
    return g_http_dispatcher;
}

void HttpDispacther::dispatch(AbstractData* data, TcpConnection* conn) {
  HttpRequest* resquest = dynamic_cast<HttpRequest*>(data);
  HttpResponse response;


  std::string url_path = resquest->m_request_path;
  if (!url_path.empty()) {
    auto it = m_servlets.find(url_path);
    if (it == m_servlets.end()) {
      NotFoundHttpServlet servlet;
      servlet.setCommParam(resquest, &response);
      servlet.handle(resquest, &response);
    } else {
      it->second->setCommParam(resquest, &response);
      it->second->handle(resquest, &response);
    }
  }

  conn->getCodec()->encode(conn->getOutBuffer().get(), &response);


}

void HttpDispacther::registerServlet(const std::string& path, HttpServlet::ptr servlet) {
  auto it = m_servlets.find(path);
  if (it == m_servlets.end()) {
    m_servlets[path] = servlet;
  } 
}




}