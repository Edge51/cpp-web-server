cmake_minimum_required(VERSION 3.16.5)

add_library(ConcurrentQueue SHARED)
target_sources(ConcurrentQueue PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ConcurrentQueue.cpp
)
target_include_directories(ConcurrentQueue PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()
add_executable(ConcurrentQueueTest)
target_sources(ConcurrentQueueTest PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/test/ConcurrentQueueTest.cpp
)
target_include_directories(ConcurrentQueueTest PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(ConcurrentQueueTest PUBLIC
    gmock
    gtest
    gtest_main
)

include(GoogleTest)