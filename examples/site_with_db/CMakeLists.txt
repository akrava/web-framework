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

    include_directories(${OPENSSL_INCLUDE_DIR})
    include_directories(${nlohmann_json_DIR}/../../../include)
	if(DEFINED ENV{BOOST_ROOT})
		include_directories(${BOOST_ROOT})
	else()
		include_directories(${Boost_DIR}/../../../include/boost-${Boost_VERSION_MAJOR}_${Boost_VERSION_MINOR})
	endif()
	include_directories(${mstch_DIR}/../../../include)

	include_directories(${akrava_web-server_DIR}/../../../include)
ENDIF()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# RELEASE
add_executable(${PROJECT_NAME} ${APP_SOURCES} ${MIDDLEWARE_SOURCES} main.cpp)
target_link_libraries(${PROJECT_NAME} akrava::akrava_web-server nlohmann_json::nlohmann_json mstch::mstch sqlite3 OpenSSL::Crypto)

# DEBUG
#add_executable(${PROJECT_NAME} ${SOURCES} ${APP_SOURCES} ${MIDDLEWARE_SOURCES} main.cpp )
#target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json mstch::mstch sqlite3 OpenSSL::Crypto)
# DEBUG