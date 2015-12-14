cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lxcb -pthread")
project(dummy_project)
include_directories(src)
include_directories(src/foo)
include_directories(src/graphics)
include_directories(src/utils)
include_directories(src/ml)
include_directories(test)
add_executable(dummy_executable
  test/LinkedListTest.cpp
  test/CArrayListTest.cpp
  src/graphics/ChartData.cpp
  test/test.cpp
)
