cmake_minimum_required(VERSION 3.5)
project(CppWebServer)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib lib)

add_executable(CppWebServer src/server/ServerMain.cpp src/server/Server.cpp
        src/server/Logger.cpp
        src/server/InetAddress.cpp
        src/server/Epoll.cpp
        src/server/Socket.cpp
        src/server/Channel.cpp
        src/server/EventLoop.cpp
        src/server/Acceptor.cpp
        src/server/Acceptor.h
        src/server/Connection.cpp
        src/server/Connection.h
        src/server/Buffer.cpp
        src/server/Buffer.h
)

target_include_directories(CppWebServer PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/server)


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
        lib/Logger/src/logger.cpp
        src/server/Epoll.cpp
        src/server/Socket.cpp
        src/server/InetAddress.cpp
        src/server/Server.cpp
        src/client/Client.cpp
        src/server/Channel.cpp
        src/server/Channel.h
        src/server/EventLoop.cpp
)
target_link_libraries(CppWebServerTest GTest::gtest_main)
target_include_directories(CppWebServerTest PRIVATE
    Logger/include
    src/server
    src/client
    test)

include(GoogleTest)
gtest_discover_tests(CppWebServerTest)