cmake_minimum_required(VERSION 3.16.5)
project(CppWebServer)

set(CMAKE_CXX_STANDARD 17)

add_library(Logger SHARED)

target_sources(Logger PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/logger.cpp)
target_include_directories(Logger PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
