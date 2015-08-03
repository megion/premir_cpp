cmake_minimum_required(VERSION 2.8)
project(cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)

include_directories(foo)

add_executable(cpp ${SOURCE_FILES})

add_subdirectory(foo)

target_link_libraries(cpp foo)