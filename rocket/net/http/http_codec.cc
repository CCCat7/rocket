#include <algorithm>
#include <sstream>
#include "http_codec.h"
#include "rocket/net/http/string_util.h"
#include "rocket/net/http/abstract_data.h"
#include "rocket/net/http/abstract_codec.h"
#include "rocket/net/http/http_request.h"
#include "rocket/net/http/http_response.h"

namespace rocket {


HttpCodeC::HttpCodeC() {

}

HttpCodeC::~HttpCodeC() {

}



void HttpCodeC::encode(TcpBuffer* buf, AbstractData* data) {
  HttpResponse* response = dynamic_cast<HttpResponse*>(data);
  response->encode_succ = false;

  std::stringstream ss;
  ss << response->m_response_version << " " << response->m_response_code << " "
    << response->m_response_info << "\r\n" << response->m_response_header.toHttpString()
    << "\r\n" << response->m_response_body;
  std::string http_res = ss.str();

  buf->writeToBuffer(http_res.c_str(), http_res.length());
  response->encode_succ = true;
}

void HttpCodeC::decode(TcpBuffer* buf, AbstractData* data) {
  std::string strs = "";
  if (!buf || !data) {
    return;
  }
  HttpRequest* request = dynamic_cast<HttpRequest*>(data);
  if (!request) {
    return;
  }

  strs = buf->getBufferString();

  bool is_parse_request_line = false;
  bool is_parse_request_header = false;
  bool is_parse_request_content = false;
  // bool is_parse_succ = false;
  int read_size = 0;
  std::string tmp(strs);
  int len = tmp.length();
  while (1) {
    if (!is_parse_request_line) {
      size_t i = tmp.find(g_CRLF);
      if (i == tmp.npos) {
        return;
      }
      if (i == tmp.length() - 2) {
        break;
      }
      is_parse_request_line = parseHttpRequestLine(request, tmp.substr(0, i));
      if (!is_parse_request_line) {
        return;
      }    
      tmp = tmp.substr(i + 2, len - 2 - i);
      len = tmp.length();
      read_size = read_size + i + 2;
    }

    if (!is_parse_request_header) {
      size_t j  = tmp.find(g_CRLF_DOUBLE);
      if (j == tmp.npos) {
        return;
      }
      // if (j == tmp.length() - 4) {
      //   DebugLog << "need to read more data";
      //   goto parse_error;
      // }
      is_parse_request_header = parseHttpRequestHeader(request, tmp.substr(0, j));
      if (!is_parse_request_header) {
        return;
      }
      tmp = tmp.substr(j + 4, len - 4 - j);
      len = tmp.length();
      read_size = read_size + j + 4;
    }
    if (!is_parse_request_content) {
      int content_len = std::atoi(request->m_requeset_header.m_maps["Content-Length"].c_str());
      if ((int)strs.length() - read_size < content_len) {
        return;
      }
      if (request->m_request_method == POST && content_len != 0) {
        is_parse_request_content = parseHttpRequestContent(request, tmp.substr(0, content_len));
        if (!is_parse_request_content) {
          return;
        }
        read_size = read_size + content_len;
      } else {
        is_parse_request_content = true;
      }

    }
    if (is_parse_request_line && is_parse_request_header && is_parse_request_header) {
      buf->recycleRead(read_size);
      break;
    }
  }

  request->decode_succ = true;
  data = request;

}


bool HttpCodeC::parseHttpRequestLine(HttpRequest* requset, const std::string& tmp) {
  size_t s1 = tmp.find_first_of(" ");
  size_t s2 = tmp.find_last_of(" ");

  if (s1 == tmp.npos || s2 == tmp.npos || s1 == s2) {
    return false;
  }
  std::string method = tmp.substr(0, s1);
  std::transform(method.begin(), method.end(), method.begin(), toupper);
  if (method == "GET") {
    requset->m_request_method = HttpMethod::GET;
  } else if (method == "POST") {
    requset->m_request_method = HttpMethod::POST;
  } else {
    return false;
  }

  std::string version = tmp.substr(s2 + 1, tmp.length() - s2 - 1);
  std::transform(version.begin(), version.end(), version.begin(), toupper);
  if (version != "HTTP/1.1" && version != "HTTP/1.0") {
    return false;
  }
  requset->m_request_version = version;
  

  std::string url = tmp.substr(s1 + 1, s2 - s1 - 1);
  size_t j = url.find("://");

  if (j != url.npos && j + 3 >= url.length()) {
    return false;
  }
  int l = 0;
  if (j == url.npos) {
  } else {
    url = url.substr(j + 3, s2 - s1  - j - 4);
    j = url.find_first_of("/");
    l = url.length();
    if (j == url.npos || j == url.length() - 1) {
      return true;
    }
    url = url.substr(j + 1, l - j - 1);
  }

  l = url.length();
  j = url.find_first_of("?");
  if (j == url.npos) {
    requset->m_request_path = url;
    return true;
  }
  requset->m_request_path = url.substr(0, j);
  requset->m_request_query = url.substr(j + 1, l - j - 1);
  StringUtil::SplitStrToMap(requset->m_request_query, "&", "=", requset->m_query_maps);
  return true;

}

bool HttpCodeC::parseHttpRequestHeader(HttpRequest* requset, const std::string& str) {
  if (str.empty() || str.length() < 4 || str == "\r\n\r\n") {
    return true;
  }
  std::string tmp = str;
  StringUtil::SplitStrToMap(tmp, "\r\n", ":", requset->m_requeset_header.m_maps);
  return true;
}
bool HttpCodeC::parseHttpRequestContent(HttpRequest* requset, const std::string& str) {
  if (str.empty()) {
    return true;
  }
  requset->m_request_body = str;
  return true;
}


ProtocalType HttpCodeC::getProtocalType() {
  return Http_Protocal;
}

}
