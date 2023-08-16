#ifndef TINYRPC_NET_HTTP_HTTP_RESPONSE_H
#define TINYRPC_NET_HTTP_HTTP_RESPONSE_H

#include <string>
#include <memory>

#include "rocket/net/http/abstract_data.h"
#include "rocket/net/http/http_define.h"

namespace rocket {

class HttpResponse : public AbstractData {
 public:
  typedef std::shared_ptr<HttpResponse> ptr; 

 public:
  std::string m_response_version;   
  int m_response_code;
  std::string m_response_info;
  HttpResponseHeader m_response_header;
  std::string m_response_body;   
};

}


#endif
