//
// Created by Edge51 on 2025/9/10.
//

#include "HttpParser.h"

#include <sstream>
#include <algorithm>

#include "Logger.h"
#include "util/StringUtil.h"

namespace http {
    HttpParser::HttpParser()
    {

    }

    HttpRequest HttpParser::Parse(const std::string &data)
    {
        HttpRequest request;
        std::stringstream ss(data);
        std::string line;
        std::getline(ss, line);
        ParseFirstLine(line, request);
        std::unordered_map<std::string, std::string> headers;
        while (std::getline(ss, line)) {
            std::string key;
            std::string value;
            ParseKeyValue(line, key, value);
            headers[key] = value;
        }
        request.SetHeaders(headers);
        return request;
    }

    void HttpParser::ParseFirstLine(const std::string &data, HttpRequest &request)
    {
        std::stringstream ss(data);
        std::string word;
        ss >> word;
        request.SetMethod(StringToMethod(word));
        ss >> word;
        request.SetUrl(word);
        ss >> word;
        request.SetVersion(StringToVersion(word));
    }

    HttpMethod HttpParser::StringToMethod(const std::string &method)
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

    HttpVersion HttpParser::StringToVersion(const std::string& version)
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

    void HttpParser::ParseKeyValue(const std::string& line, std::string& key, std::string& value)
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