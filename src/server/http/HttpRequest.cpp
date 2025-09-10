//
// Created by Edge51 on 2025/9/10.
//

#include "HttpRequest.h"

namespace http {


HttpVersion HttpRequest::GetVersion() const
{
    return m_version;
}

void HttpRequest::SetVersion(HttpVersion version)
{
    m_version = version;
}

HttpMethod HttpRequest::GetMethod() const
{
    return m_method;
}

void HttpRequest::SetMethod(HttpMethod method)
{
    m_method = method;
}

std::string HttpRequest::GetUrl() const
{
    return m_url;
}

void HttpRequest::SetUrl(const std::string &url)
{
    m_url = url;
}

std::unordered_map<std::string, std::string> HttpRequest::GetHeaders() const
{
    return m_headers;
}

void HttpRequest::SetHeaders(const std::unordered_map<std::string, std::string> &headers)
{
    m_headers = headers;
}

std::string HttpRequest::GetQuery() const
{
    return m_query;
}

void HttpRequest::SetQuery(const std::string &query)
{
    m_query = query;
}

std::string HttpRequest::GetBody() const
{
    return m_body;
}

void HttpRequest::SetBody(const std::string &body)
{
    m_body = body;
}

} // namespace http
