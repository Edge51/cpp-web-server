cmake_minimum_required(VERSION 3.5)
project(CppWebServer)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib lib)

add_executable(CppWebServer src/server/EchoServer.cpp src/server/tcp/TcpServer.cpp
        src/server/tcp/Logger.cpp
        src/server/tcp/InetAddress.cpp
        src/server/tcp/Epoll.cpp
        src/server/tcp/Socket.cpp
        src/server/tcp/Channel.cpp
        src/server/tcp/EventLoop.cpp
        src/server/tcp/Acceptor.cpp
        src/server/tcp/Acceptor.h
        src/server/tcp/TcpConnection.cpp
        src/server/tcp/TcpConnection.h
        src/server/tcp/Buffer.cpp
        src/server/tcp/Buffer.h
        src/server/tcp/EventLoopThreadPool.cpp
        src/server/tcp/EventLoopThreadPool.h
        src/server/tcp/EventLoopThread.cpp
        src/server/tcp/EventLoopThread.h
        src/server/http/HttpRequest.cpp
        src/server/http/HttpRequest.h
        src/server/http/HttpParser.cpp
        src/server/http/HttpParser.h
        src/server/util/StringUtil.cpp
        src/server/util/StringUtil.h
        src/server/http/HttpResponse.cpp
        src/server/http/HttpResponse.h
        src/server/http/HttpDefine.h
        src/server/http/HttpServer.cpp
        src/server/http/HttpServer.h
)

target_include_directories(CppWebServer PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/server/http
    ${CMAKE_CURRENT_SOURCE_DIR}/src/server/util
    ${CMAKE_CURRENT_SOURCE_DIR}/src/server/tcp)


include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        f8d7d77c06936315286eb55f8de22cd23c188571 # release-1.14.0
    #   googletest
    # URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(CppWebServerTest
        test/CppWebServerTest.cpp
        src/server/tcp/Epoll.cpp
        src/server/tcp/Socket.cpp
        src/server/tcp/InetAddress.cpp
        src/server/tcp/TcpServer.cpp
        src/server/tcp/TcpConnection.cpp
        src/server/tcp/Acceptor.cpp
        src/server/tcp/Buffer.cpp
        src/server/tcp/EventLoopThreadPool.cpp
        src/server/tcp/EventLoopThread.cpp
        src/client/Client.cpp
        src/server/tcp/Channel.cpp
        src/server/tcp/Channel.h
        src/server/tcp/EventLoop.cpp
        src/server/util/StringUtil.cpp
        src/server/http/HttpParser.cpp
        src/server/http/HttpParser.h
        src/server/http/HttpRequest.cpp
        src/server/http/HttpRequest.h
        src/server/http/HttpResponse.cpp
        src/server/http/HttpResponse.h
        test/HttpRequestTest.cpp
        test/HttpRequestTest.h
        test/HttpResponseTest.cpp
        test/HttpResponseTest.h
)
target_link_libraries(CppWebServerTest GTest::gtest_main)
target_include_directories(CppWebServerTest PRIVATE
    Logger/include
    src/server/tcp
    src/server/http
    src/server/util
    src/client
    test)

include(GoogleTest)
gtest_discover_tests(CppWebServerTest)