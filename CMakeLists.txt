cmake_minimum_required(VERSION 3.8)

project(akrava_web-server VERSION 1.0.1 LANGUAGES CXX)

find_package(nlohmann_json REQUIRED)
find_package(mstch REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_library(akrava_web-server STATIC ${SOURCES})
set_property(TARGET akrava_web-server PROPERTY VERSION ${akrava_web-server_VERSION})
#target_link_libraries(akrava_web-server nlohmann_json::nlohmann_json mstch::mstch)

install(TARGETS akrava_web-server EXPORT akrava_web-serverTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(FILES "${PROJECT_SOURCE_DIR}/include/app.h"
        "${PROJECT_SOURCE_DIR}/include/context.h"
        "${PROJECT_SOURCE_DIR}/include/response.h"
        "${PROJECT_SOURCE_DIR}/include/request.h"
        "${PROJECT_SOURCE_DIR}/include/middleware.h"
        "${PROJECT_SOURCE_DIR}/include/handler.h"
        "${PROJECT_SOURCE_DIR}/include/socket.h"
        "${PROJECT_SOURCE_DIR}/include/redirect_response.h"
        "${PROJECT_SOURCE_DIR}/include/log_manager.h"
        "${PROJECT_SOURCE_DIR}/include/init_params.h"
        "${PROJECT_SOURCE_DIR}/include/http.h"
        "${PROJECT_SOURCE_DIR}/include/headers.h"
        "${PROJECT_SOURCE_DIR}/include/message_body.h"
        "${PROJECT_SOURCE_DIR}/include/uri.h"
        "${PROJECT_SOURCE_DIR}/include/file_handler.h"
        "${PROJECT_SOURCE_DIR}/include/html_middleware.h"
        "${PROJECT_SOURCE_DIR}/include/cookie_middleware.h"
        "${PROJECT_SOURCE_DIR}/include/cookie_entity.h"
        "${PROJECT_SOURCE_DIR}/include/parser_http.h"
        "${PROJECT_SOURCE_DIR}/include/form_middleware.h"
        "${PROJECT_SOURCE_DIR}/include/json_middleware.h"
        "${PROJECT_SOURCE_DIR}/include/runtime_exception.h"
        DESTINATION include/akrava/web-server
        COMPONENT Devel)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/akrava_web-server/akrava_web-server-config-version.cmake"
        VERSION ${akrava_web-server_VERSION}
        COMPATIBILITY SameMajorVersion)

export(EXPORT akrava_web-serverTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/akrava_web-server/akrava_web-server-targets.cmake"
       NAMESPACE akrava::)

configure_file("${PROJECT_SOURCE_DIR}/akrava_web-server-config.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/akrava_web-server/akrava_web-server-config.cmake")

configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/akrava_web-server-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/akrava_web-server/akrava_web-server-config.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(EXPORT akrava_web-serverTargets
        FILE akrava_web-server-targets.cmake
        NAMESPACE akrava::
        DESTINATION lib/cmake/akrava_web-server)

install(FILES
        "${PROJECT_SOURCE_DIR}/akrava_web-server-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/akrava_web-server/akrava_web-server-config-version.cmake"
        DESTINATION lib/cmake/akrava_web-server
        COMPONENT Devel)