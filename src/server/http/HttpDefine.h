//
// Created by Edge51 on 2025/9/16.
//

#ifndef CPPWEBSERVER_HTTPDEFINE_H
#define CPPWEBSERVER_HTTPDEFINE_H

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

    enum class HttpStatusCode {
        UNDEFINED = 0,
        OK = 200,
        BAD_REQUEST = 400,
        NOT_FOUND = 404,
    };

} // namespace http

#endif //CPPWEBSERVER_HTTPDEFINE_H