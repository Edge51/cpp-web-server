//
// Created by Edge51 on 2025/9/10.
//

#ifndef CPPWEBSERVER_HTTPPARSER_H
#define CPPWEBSERVER_HTTPPARSER_H
#include <memory>

#include "HttpRequest.h"

namespace http {

class HttpParser {
public:
    using ptr = std::shared_ptr<HttpParser>;
    HttpParser();
    HttpRequest Parse(const std::string& data);
private:
    void ParseFirstLine(const std::string& data, HttpRequest& request);
    HttpMethod StringToMethod(const std::string& method);
    HttpVersion StringToVersion(const std::string& version);
    void ParseKeyValue(const std::string& line, std::string& key, std::string& value);
};

} // namespace http

#endif //CPPWEBSERVER_HTTPPARSER_H