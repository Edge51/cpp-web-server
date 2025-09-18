//
// Created by Edge51 on 2025/9/10.
//

#include "HttpRequestParser.h"

#include <sstream>
#include <algorithm>

#include "Logger.h"
#include "StringUtil.h"

namespace http {

    void HttpRequestParser::AppendData(const std::string &data)
    {
        m_data += data;
    }


    /** @input: request input, may be incomplete
     *  @output: HttpRequest object
     *
     */
    void HttpRequestParser::Parse()
    {
        m_state = ParseState::REQUEST_LINE;
        if (m_state == ParseState::REQUEST_LINE) {
            size_t pos = m_data.find("\r\n");
            if (pos == std::string::npos) {
                LOG("Parsing request line, waiting for more data");
                return ;
            }
            ParseRequestLine(m_data.substr(0, pos));
            m_data.erase(0, pos + 2);
            m_state = ParseState::HEADER_FIELD;
        }
        if (m_state == ParseState::HEADER_FIELD) {
            size_t pos = m_data.find("\r\n");
            while (pos != std::string::npos) {
                if (pos == 0) {
                    m_data.erase(0, 2);
                    PrepareBodyParsing();
                    m_state = ParseState::CONTENT;
                    break;
                }
                ParseHeaderLine(m_data.substr(0, pos));
                m_data.erase(0, pos + 2);
                pos = m_data.find("\r\n");
            }
        }
        if (m_state == ParseState::CONTENT) {
            if (m_data.length() >= m_contentLength) {
                ParseContent();
                m_state = ParseState::GOOD_REQUEST;
            }
        }
    }

    HttpRequest HttpRequestParser::GetHttpRequest()
    {
        HttpRequest request;
        request.SetMethod(m_method);
        request.SetUrl(m_url);
        request.SetVersion(m_version);
        request.SetHeaders(m_headers);
        request.SetQuery(m_query);
        request.SetBody(m_body);
        return request;
    }

    bool HttpRequestParser::IsComplete() const
    {
        return m_state == ParseState::BAD_REQUEST || m_state == ParseState::GOOD_REQUEST;
    }

    bool HttpRequestParser::HasError() const
    {
        return m_state == ParseState::BAD_REQUEST;
    }

    bool HttpRequestParser::IsValidUrl(const std::string &url)
    {
        return true; // TODO
    }

    void HttpRequestParser::ParseRequestLine(const std::string &requestLine)
    {
        size_t firstBlank = requestLine.find(' ');
        size_t secondBlank = requestLine.find(' ', firstBlank + 1);
        m_method = StringToMethod(requestLine.substr(0, firstBlank));
        m_url = requestLine.substr(firstBlank + 1, secondBlank - firstBlank - 1);
        m_version = StringToVersion(requestLine.substr(secondBlank + 1));
    }

    void HttpRequestParser::ParseHeaderLine(const std::string &requestLine)
    {
        std::string key;
        std::string value;
        ParseKeyValue(requestLine, key, value);
        m_headers[key] = value;
    }

    void HttpRequestParser::PrepareBodyParsing()
    {
        auto it = m_headers.find("Content-Length");
        if (it != m_headers.end()) {
            m_contentLength = std::stoul(m_headers["Content-Length"]);
        } else {
            m_contentLength = 0;
        }
    }
    void HttpRequestParser::ParseContent()
    {
        m_body = m_data.substr(0, m_contentLength);
        m_state = ParseState::GOOD_REQUEST;
    }

    HttpMethod HttpRequestParser::StringToMethod(const std::string &method)
    {
        if (method == "GET") {
            return HttpMethod::GET;
        } else if (method == "POST") {
            return HttpMethod::POST;
        } else if (method == "PUT") {
            return HttpMethod::PUT;
        } else if (method == "DELETE") {
            return HttpMethod::DELETE;
        }
        LOG("[WARN] method[%s] parse failed", method.c_str());
        return HttpMethod::UNDEFINED;
    }

    HttpVersion HttpRequestParser::StringToVersion(const std::string& version)
    {
        if (version == "HTTP/1.1") {
            return HttpVersion::HTTP_1_1;
        } else if (version == "HTTP/1.0") {
            return HttpVersion::HTTP_1_0;
        } else if (version == "HTTP/0.0") {
            return HttpVersion::HTTP_0_0;
        }
        LOG("[WARN] version[%s] parse failed", version.c_str());
        return HttpVersion::UNDEFINED;
    }

    void HttpRequestParser::ParseKeyValue(const std::string& line, std::string& key, std::string& value)
    {
        std::string trimLine = StringUtil::Trim(line);
        if (trimLine.empty()) {
            return ;
        }
        if (trimLine.find(':') == std::string::npos) {
            LOG("[ERROR] line[%s] with no ':'", line.c_str());
            return;
        }
        key = trimLine.substr(0, trimLine.find(':'));
        value = trimLine.substr(trimLine.find(':') + 1);
        value = StringUtil::Trim(value);
    }


} // namespace http