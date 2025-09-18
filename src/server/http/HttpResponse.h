//
// Created by Edge51 on 2025/9/16.
//

#ifndef CPPWEBSERVER_HTTPRESPONSE_H
#define CPPWEBSERVER_HTTPRESPONSE_H
#include <memory>
#include <string>
#include <unordered_map>

#include "HttpDefine.h"

namespace http {

class HttpResponse {
public:
    using ptr = std::shared_ptr<HttpResponse>;
    HttpResponse() = default;
    void SetContentType(const std::string& contentType);
    void SetHttpVersion(HttpVersion version);
    void SetStatusCode(int status_code);
    void SetStatusDescription(const std::string& statusDescription);
    void SetBody(const std::string& body);
    void SetCloseFlag(bool closeFlag);
    std::string PlainText() const;
private:
    static HttpStatusCode IntToHttpStatusCode(int status_code);
    static std::string StatusCodeToString(HttpStatusCode code);
private:
    HttpVersion m_version { HttpVersion::UNDEFINED };
    HttpStatusCode m_statusCode { HttpStatusCode::UNDEFINED };
    std::unordered_map<std::string, std::string> m_headers;
    std::string m_statusDescription;
    std::string m_body;
    bool m_isCloseResponse { false };
};

} // namespace http

#endif //CPPWEBSERVER_HTTPRESPONSE_H