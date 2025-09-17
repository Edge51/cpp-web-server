//
// Created by Edge51 on 2025/9/16.
//

#include "HttpResponse.h"

namespace http {


void HttpResponse::SetContentType(const std::string &contentType)
{
    m_headers["Content-Type"] = contentType;
}

void HttpResponse::SetHttpVersion(HttpVersion version)
{
    m_version = version;
}

void HttpResponse::SetStatusCode(int status_code)
{
    m_statusCode = IntToHttpStatusCode(status_code);
}

void HttpResponse::SetStatusDescription(const std::string &statusDescription)
{
    m_statusDescription = statusDescription;
}

void HttpResponse::SetBody(const std::string &body)
{
    m_body = body;
}

void HttpResponse::SetCloseFlag(bool closeFlag)
{
    m_isCloseResponse = closeFlag;
}

std::string HttpResponse::PlainText() const
{
    std::string plainText = "";
    plainText += "HTTP/1.1 ";
    plainText += StatusCodeToString(m_statusCode) + "\r\n";
    if (m_isCloseResponse) {
        plainText += "Connection: close\r\n";
    } else {
        plainText += "Content-Length: " + std::to_string(m_body.size()) + "\r\n";
        plainText += "Connection: keep-alive\r\n";
    }
    for (const auto &header : m_headers) {
        plainText += header.first + ": " + header.second + "\r\n";
    }
    plainText += "\r\n";
    plainText += m_body;
    return plainText;
}

HttpStatusCode HttpResponse::IntToHttpStatusCode(int status_code)
{
    if (status_code == static_cast<int>(HttpStatusCode::OK)) {
        return HttpStatusCode::OK;
    }
    return HttpStatusCode::UNDEFINED;
}

std::string HttpResponse::StatusCodeToString(HttpStatusCode code)
{
    switch (code) {
        case HttpStatusCode::OK:
            return "200 OK";
        case HttpStatusCode::NOT_FOUND:
            return "404 Not Found";
        case HttpStatusCode::BAD_REQUEST:
            return "400 Bad Request";
        default:
            return "500 Internal Server Error";
    }
}

} // http