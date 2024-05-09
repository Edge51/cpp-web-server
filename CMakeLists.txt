cmake_minimum_required(VERSION 3.0)
project(CppWebServer)

set(CMAKE_CXX_STANDARD 17)

add_executable(CppWebServer src/server/server.cpp)

target_include_directories(CppWebServer PUBLIC
    ${CMAKE_SCMAKE_CURRENT_SOURCE_DIR}/include)


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
add_executable(CppWebServerTest test/CppWebServerTest.cpp test/CppWebServerTest.h)
target_link_libraries(CppWebServerTest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(CppWebServerTest)