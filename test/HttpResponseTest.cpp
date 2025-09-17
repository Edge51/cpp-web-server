//
// Created by Edge51 on 2025/9/17.
//

#include "HttpResponseTest.h"
#include "HttpResponse.h"

TEST_F(HttpResponseTest, object_to_plaintext)
{
    http::HttpResponse httpResponse;
    httpResponse.SetStatusCode(200);
    httpResponse.SetHttpVersion(http::HttpVersion::HTTP_1_1);
    httpResponse.SetContentType("application/json");
    httpResponse.SetStatusDescription("OK");
    httpResponse.SetBody("hello world");

    std::string expectText = "HTTP/1.1 200 OK\r\n"
        "Content-Length: 11\r\n"
        "Connection: keep-alive\r\n"
        "Content-Type: application/json\r\n"
        "\r\n"
        "hello world";
    EXPECT_EQ(httpResponse.PlainText(), expectText);
}