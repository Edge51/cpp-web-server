//
// Created by Edge51 on 2025/9/10.
//

#ifndef CPPWEBSERVER_HTTPREQUEST_H
#define CPPWEBSERVER_HTTPREQUEST_H
#include <string>
#include <unordered_map>

namespace http {

enum class HttpMethod {
    UNDEFINED = 0,
    GET,
    POST,
    PUT,
    DELETE
};

enum class HttpVersion {
    UNDEFINED = 0,
    HTTP_0_0,
    HTTP_1_0,
    HTTP_1_1
};

class HttpRequest {
public:
    HttpRequest() = default;
    HttpVersion GetVersion() const;
    void SetVersion(HttpVersion version);
    HttpMethod GetMethod() const;
    void SetMethod(HttpMethod method);
    std::string GetUrl() const;
    void SetUrl(const std::string &url);
    std::unordered_map<std::string, std::string> GetHeaders() const;
    void SetHeaders(const std::unordered_map<std::string, std::string> &headers);
    std::string GetQuery() const;
    void SetQuery(const std::string &query);
    std::string GetBody() const;
    void SetBody(const std::string &body);

private:
    HttpMethod m_method { HttpMethod::UNDEFINED };
    HttpVersion m_version { HttpVersion::UNDEFINED };
    std::unordered_map<std::string, std::string> m_headers;
    std::string m_url;
    std::string m_query;
    std::string m_body;
};

}  // namespace http

#endif //CPPWEBSERVER_HTTPREQUEST_H