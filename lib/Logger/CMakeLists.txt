cmake_minimum_required(VERSION 3.16.5)
project(CppWebServer)

set(CMAKE_CXX_STANDARD 17)

add_library(Logger SHARED
        src/FixedBuffer.cpp
        src/FixedBuffer.h
        include/DisableCopy.h
        src/LogFormatter.cpp
        src/LogFormatter.h
        test/LoggerTest.cpp)

target_sources(Logger PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/logger.cpp
)
target_include_directories(Logger PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_definitions(Logger PRIVATE LOGGER_EXPORTS)

add_executable(LoggerTest)
target_sources(LoggerTest PRIVATE
        test/LoggerTest.cpp
)
target_include_directories(LoggerTest PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(LoggerTest PRIVATE Logger GTest::gtest_main)