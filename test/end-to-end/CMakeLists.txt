cmake_minimum_required(VERSION 3.10)

project(EndToEndTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TREE_INCLUDE_DIR ../../include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_executable(${PROJECT_NAME} end-to-end.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${TREE_INCLUDE_DIR})