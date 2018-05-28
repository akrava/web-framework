cmake_minimum_required(VERSION 3.10)

project(web-server)
include_directories(include)
set(CMAKE_CXX_STANDARD 11)
file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} main.cpp
        src/socket.cpp include/socket.h
        src/app.cpp include/app.h
        src/init_params.cpp
        include/init_params.h include/runtime_exception.h src/handler.cpp include/handler.h src/context.cpp include/context.h src/log_manager.cpp include/log_manager.h src/uri.cpp include/uri.h src/headers.cpp include/headers.h src/message_body.cpp include/message_body.h src/parser_http.cpp include/parser_http.h)