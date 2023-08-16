#ifndef TINYRPC_NET_HTTP_HTTP_CODEC_H
#define TINYRPC_NET_HTTP_HTTP_CODEC_H

#include <map>
#include <string>
#include "rocket/net/http/abstract_data.h"
#include "rocket/net/http/abstract_codec.h"
#include "rocket/net/http/http_request.h"

namespace rocket {

class HttpCodeC : public AbstractCodeC {
 public:
  HttpCodeC();

  ~HttpCodeC();

  void encode(TcpBuffer* buf, AbstractData* data);
  
  void decode(TcpBuffer* buf, AbstractData* data);

  ProtocalType getProtocalType();

 private:
  bool parseHttpRequestLine(HttpRequest* requset, const std::string& tmp);
  bool parseHttpRequestHeader(HttpRequest* requset, const std::string& tmp);
  bool parseHttpRequestContent(HttpRequest* requset, const std::string& tmp);
};

} 


#endif
