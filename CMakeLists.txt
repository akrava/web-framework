cmake_minimum_required(VERSION 3.8)

project(akrava_web-server VERSION 2.0.0 LANGUAGES CXX)

find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(mstch REQUIRED)
find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
	set(OPENSSL_USE_STATIC_LIBS TRUE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

if(WIN32)
	include_directories(${OPENSSL_INCLUDE_DIR})
    include_directories(${nlohmann_json_DIR}/../../../include)
	if(DEFINED ENV{BOOST_ROOT})
		include_directories(${BOOST_ROOT}/include)
	else()
		include_directories(${Boost_DIR}/../../../include/boost-${Boost_VERSION_MAJOR}_${Boost_VERSION_MINOR})
	endif()
	include_directories(${mstch_DIR}/../../../include)
endif()

include_directories("include")
file(GLOB SOURCES "src/*.cpp")
add_library(akrava_web-server STATIC ${SOURCES})
set_property(TARGET akrava_web-server PROPERTY VERSION ${akrava_web-server_VERSION})

install(
    TARGETS akrava_web-server EXPORT akrava_web-serverTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

file (GLOB HEADERS_FILES ${PROJECT_SOURCE_DIR}/include/*)

install(
    FILES
    ${HEADERS_FILES}
    DESTINATION include/akrava/web-server
    COMPONENT Devel
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/akrava_web-server/akrava_web-server-config-version.cmake"
    VERSION ${akrava_web-server_VERSION}
    COMPATIBILITY SameMajorVersion
)

export(
    EXPORT akrava_web-serverTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/akrava_web-server/akrava_web-server-targets.cmake"
    NAMESPACE akrava::
)

configure_file(
    "scripts/akrava_web-server-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/akrava_web-server/akrava_web-server-config.cmake"
)

configure_package_config_file(
    "scripts/akrava_web-server-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/akrava_web-server/akrava_web-server-config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(
    EXPORT akrava_web-serverTargets
    FILE akrava_web-server-targets.cmake
    NAMESPACE akrava::
    DESTINATION lib/cmake/akrava_web-server
)

install(
    FILES
    "scripts/akrava_web-server-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/akrava_web-server/akrava_web-server-config-version.cmake"
    DESTINATION lib/cmake/akrava_web-server
    COMPONENT Devel
)