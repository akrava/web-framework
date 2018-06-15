cmake_minimum_required(VERSION 3.8)

project(simple-site)
find_package(akrava_web-server)
find_package(mstch)
find_package(nlohmann_json)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

add_executable(${PROJECT_NAME} main.cpp DatabaseMiddleware.cpp DatabaseMiddleware.h)

target_link_libraries(${PROJECT_NAME} akrava::akrava_web-server nlohmann_json::nlohmann_json mstch::mstch sqlite3)