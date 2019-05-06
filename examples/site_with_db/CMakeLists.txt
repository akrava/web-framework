cmake_minimum_required(VERSION 3.8)

project(simple-site)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "C:\\Program Files\\CMake\\share\\cmake-3.14\\Modules")

find_package(akrava_web-server)
find_package(mstch)
find_package(nlohmann_json)
find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
	set(OPENSSL_USE_STATIC_LIBS TRUE)
endif()

file(GLOB APP_SOURCES "src/*.cpp")
file(GLOB MIDDLEWARE_SOURCES "middleware/*.cpp")
include_directories("include")

# DEBUG
#include_directories("./../../include")
#file(GLOB SOURCES "./../../src/*.cpp")
# DEBUG

IF (WIN32)
	link_directories("C:\\Program Files\\sqlite3")
	include_directories("C:\\Program Files\\sqlite3")
	include_directories("C:\\Program Files\\OpenSSL-Win64\\include")
    include_directories("C:\\Program Files (x86)\\nlohmann_json\\include")
	include_directories("C:\\Program Files\\boost\\include\\boost-1_70")
	include_directories("C:\\Program Files (x86)\\mstch\\include")
	include_directories("C:\\Program Files (x86)\\akrava_web-server\\include")
ENDIF()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

#add_library(LIB SHARED libpqxx::pqxx_shared)
# RELEASE
add_executable(${PROJECT_NAME} ${APP_SOURCES} ${MIDDLEWARE_SOURCES} main.cpp)
target_link_libraries(${PROJECT_NAME} akrava::akrava_web-server nlohmann_json::nlohmann_json mstch::mstch sqlite3 OpenSSL::Crypto)

# DEBUG
#add_executable(${PROJECT_NAME} ${SOURCES} ${APP_SOURCES} ${MIDDLEWARE_SOURCES} main.cpp )
#target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json mstch::mstch sqlite3 OpenSSL::Crypto)
# DEBUG


#target_include_directories(${PROJECT_NAME} PUBLIC "C:\\Program Files\\PostgreSQL\\11\\lib")

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
#        "C:\\Program Files\\libpqxx\\bin\\pqxx.dll"                       # <--this is in-file
#        $<TARGET_FILE_DIR:${PROJECT_NAME}>)        # <--this is out-file path
#
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
#        "C:\\Program Files\\PostgreSQL\\11\\lib\\libpq.dll"                       # <--this is in-file
#        $<TARGET_FILE_DIR:${PROJECT_NAME}>)        # <--this is out-file path