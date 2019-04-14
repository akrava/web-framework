cmake_minimum_required(VERSION 3.8)

project(simple-site)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "C:\\Program Files\\CMake\\share\\cmake-3.14\\Modules")

find_package(akrava_web-server)
find_package(mstch)
find_package(nlohmann_json)
find_package(Boost REQUIRED)

IF (WIN32)
	include_directories("C:\\Program Files (x86)\\nlohmann_json\\include")
	include_directories("C:\\Program Files\\boost\\include\\boost-1_70")
	include_directories("C:\\Program Files (x86)\\mstch\\include")
	include_directories("C:\\Program Files (x86)\\akrava_web-server\\include")
ENDIF()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

#add_library(LIB SHARED libpqxx::pqxx_shared)
add_executable(${PROJECT_NAME} main.cpp DatabaseMiddleware.cpp DatabaseMiddleware.h)
target_link_libraries(${PROJECT_NAME} akrava::akrava_web-server nlohmann_json::nlohmann_json mstch::mstch sqlite3)

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