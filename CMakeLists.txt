cmake_minimum_required(VERSION 3.10)

project(web-server)
include_directories(include)
set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} main.cpp
        src/sockets.cpp include/sockets.h
        src/app.cpp include/app.h
        src/init_params.cpp
        include/init_params.h include/runtime_exception.h)