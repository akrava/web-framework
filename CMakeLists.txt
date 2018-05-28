cmake_minimum_required(VERSION 3.10)

project(web-server)
include_directories(include)
set(CMAKE_CXX_STANDARD 11)
file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})
