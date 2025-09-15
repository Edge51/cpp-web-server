//
// Created by Edge51 on 2025/9/10.
//

#include "HttpRequestTest.h"
#include "HttpRequest.h"
#define private public
#include "HttpParser.h"
#undef private

using namespace http;

TEST_F(HttpRequestTest, parse_state_machine)
{
    std::string requestStr = "GET /hello?a=2 HTTP/1.1\r\n"
                      "Content-Type: application/json\r\n"
                      "Host: 127.0.0.1:1234\r\n"
                      "Connection: keep-alive\r\n"
                      "Cache-Control: max-age=0\r\n"
                      "sec-ch-ua: \"Google Chrome\";v=\"113\", \"Chromium\";v=\"113\", \"Not-A.Brand\";v=\"24\"\r\n"
                      "sec-ch-ua-mobile: ?0\r\n"
                      "sec-ch-ua-platform: \"Linux\"\r\n"
                      "Upgrade-Insecure-Requests: 1\r\n"
                      "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/113.0.0.0 Safari/537.36\r\n"
                      "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7\r\n"
                      "Sec-Fetch-Site: none\r\n"
                      "Sec-Fetch-Mode: navigate\r\n"
                      "Sec-Fetch-User: ?1\r\n"
                      "Sec-Fetch-Dest: document\r\n"
                      "Accept-Encoding: gzip, deflate, br\r\n"
                      "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,zh-TW;q=0.7\r\n"
                      "Cookie: username-127-0-0-1-8888=\"2|1:0|10:1681994652|23:username-127-0-0-1-8888|44:Yzg5ZjA1OGU0MWQ1NGNlMWI2MGQwYTFhMDAxYzY3YzU=|6d0b051e144fa862c61464acf2d14418d9ba26107549656a86d92e079ff033ea\"; _xsrf=2|dd035ca7|e419a1d40c38998f604fb6748dc79a10|168199465\r\n"
                      "\r\n";
    HttpRequestParser parser;
    parser.AppendData(requestStr);
    // GTEST_LOG_(INFO) << "parser.data[" << parser.m_data << "]";
    parser.Parse();
    // GTEST_LOG_(INFO) << "parser.m_url[" << parser.m_url << "]";
    HttpRequest httpRequest = parser.GetHttpRequest();
    EXPECT_EQ(parser.HasError(), false);
    EXPECT_EQ(parser.IsComplete(), true);
    EXPECT_EQ(httpRequest.GetMethod(), HttpMethod::GET);
    EXPECT_EQ(httpRequest.GetVersion(), HttpVersion::HTTP_1_1);
    EXPECT_EQ(httpRequest.GetUrl(), "/hello?a=2");
    std::unordered_map<std::string, std::string> headers = httpRequest.GetHeaders();
    EXPECT_EQ(headers["Content-Type"], "application/json");
    EXPECT_EQ(headers["User-Agent"], "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/113.0.0.0 Safari/537.36");
}