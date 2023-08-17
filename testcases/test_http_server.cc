#include <google/protobuf/service.h>
#include "rocket/net/http/http_request.h"
#include "rocket/net/http/http_response.h"
#include "rocket/net/http/http_servlet.h"
#include "rocket/net/http/http_define.h"
#include <assert.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <memory>
#include <unistd.h>
#include "rocket/common/log.h"
#include "rocket/common/config.h"
#include "rocket/common/log.h"
#include "rocket/net/tcp/tcp_client.h"
#include "rocket/net/tcp/net_addr.h"
#include "rocket/net/coder/string_coder.h"
#include "rocket/net/coder/abstract_protocol.h"
#include "rocket/net/coder/tinypb_coder.h"
#include "rocket/net/coder/tinypb_protocol.h"
#include "rocket/net/tcp/net_addr.h"
#include "rocket/net/tcp/tcp_server.h"
#include "rocket/net/rpc/rpc_dispatcher.h"
#include "rocket/net/http/http_dispatcher.h"
#include <sstream>

const char* html = "<html><body><h1>Welcome to TinyRPC, just enjoy it!</h1><p>%s</p></body></html>";


class QPSHttpServlet : public rocket::HttpServlet {
public:
    QPSHttpServlet() = default;
    ~QPSHttpServlet() = default;

    void handle(rocket::HttpRequest* req, rocket::HttpResponse* res) {
        setHttpCode(res, rocket::HTTP_OK);
        setHttpContentType(res, "text/html;charset=utf-8");

        std::stringstream ss;
        ss << "QPSHttpServlet Echo Success!!";
        char buf[512];
        sprintf(buf, html, ss.str().c_str());
        setHttpBody(res, std::string(buf));
    }

    std::string getServletName() {
        return "QPSHttpServlet";
    }
};


// int main(int argc, char* argv[]) {
//   if (argc != 2) {
//     printf("Start TinyRPC server error, input argc is not 2!");
//     printf("Start TinyRPC server like this: \n");
//     printf("./server a.xml\n");
//     return 0;
//   }

//   TcpServer::ptr gRpcServer;
//   rocket::InitConfig(argv[1]);

//   REGISTER_HTTP_SERVLET("/qps", QPSHttpServlet);

//   rocket::StartRpcServer();
//   return 0;
// }


    




int main() {

    rocket::Config::SetGlobalConfig("../conf/rocket.xml");
    rocket::IPNetAddr::s_ptr addr = std::make_shared<rocket::IPNetAddr>("127.0.0.1", 20000);
    rocket::TcpServer tcp_server(addr);
    rocket::Logger::InitGlobalLogger();

    //REGISTER_HTTP_SERVLET("/qps", QPSHttpServlet);
    dynamic_cast<rocket::HttpDispacther*>(tcp_server.m_http_dispatcher.get())->m_servlets["qps"] = std::make_shared<QPSHttpServlet>();
    
    tcp_server.start();

    return 0;
}