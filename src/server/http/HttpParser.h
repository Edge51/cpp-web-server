//
// Created by Edge51 on 2025/9/10.
//

#ifndef CPPWEBSERVER_HTTPPARSER_H
#define CPPWEBSERVER_HTTPPARSER_H
#include <memory>

#include "HttpRequest.h"

namespace http {

class HttpRequestParser {
public:
    using ptr = std::shared_ptr<HttpRequestParser>;
    HttpRequestParser() = default;
    void AppendData(const std::string &data);
    void Parse();
    HttpRequest GetHttpRequest();
    bool IsComplete() const;
    bool HasError() const;
private:
    enum class LineState {
        LINE_OK,
        LINE_BAD,
        LINE_OPEN,
    };

    enum class ParseState {
        REQUEST_LINE,
        HEADER_FIELD,
        CONTENT,
        BAD_REQUEST,
        GOOD_REQUEST,
    };
    void ParseRequestLine(const std::string& requestLine);
    void ParseHeaderLine(const std::string &requestLine);
    void ParseContent();
    void PrepareBodyParsing();
    bool IsValidUrl(const std::string& url);
    HttpMethod StringToMethod(const std::string& method);
    HttpVersion StringToVersion(const std::string& version);
    void ParseKeyValue(const std::string& line, std::string& key, std::string& value);
private:
    ParseState m_state {ParseState::REQUEST_LINE};
    std::string m_data;
    HttpMethod m_method { HttpMethod::UNDEFINED };
    HttpVersion m_version { HttpVersion::UNDEFINED };
    std::unordered_map<std::string, std::string> m_headers;
    std::string m_url;
    std::string m_query;
    std::string m_body;
    size_t m_contentLength { 0 };
};

} // namespace http

#endif //CPPWEBSERVER_HTTPPARSER_H