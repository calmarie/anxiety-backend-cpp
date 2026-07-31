cmake_minimum_required(VERSION 3.12...3.31)
project(anxiety_backend CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(DownloadUserver)

set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "Provide asynchronous driver for PostgreSQL" FORCE)
find_package(
    userver
    COMPONENTS core #
               postgresql
    QUIET
)
if(NOT userver_FOUND)
    # Tries TRY_DIR first, falls back to downloading userver from GitHub using CPM.
    download_userver(TRY_DIR third_party/userver)
endif()

userver_setup_environment()

# Common sources
include_directories(src)

add_library(
    ${PROJECT_NAME}_objs OBJECT
    src/handlers/bd_ping.cpp
    src/handlers/user_crud/get_user.cpp
    src/handlers/user_crud/create_user.cpp
    src/handlers/user_crud/delete_user.cpp
    src/repositories/user_repository.cpp
    src/handlers/user_crud/update_user_name.cpp
    src/service/user_service.cpp
    src/serializers/user_serializer.cpp
    src/security/password_hasher.cpp
    src/handlers/auth/login.cpp
    src/handlers/auth/register.cpp
    src/security/jwt_service.cpp
    src/service/auth_service.cpp
    src/security/refresh_token_gen.cpp
    src/repositories/refresh_token_repository.cpp

)

find_library(ARGON2_LIBRARY argon2 REQUIRED)

find_package(OpenSSL REQUIRED)

include(FetchContent)

find_package(jwt-cpp CONFIG QUIET)

if(NOT jwt-cpp_FOUND)
    FetchContent_Declare(
        jwt-cpp
        GIT_REPOSITORY https://github.com/Thalhammer/jwt-cpp.git
        GIT_TAG v0.7.1
    )
    FetchContent_MakeAvailable(jwt-cpp)
endif()

target_link_libraries(
    ${PROJECT_NAME}_objs
    PUBLIC userver-core
           userver-postgresql
           userver-universal
           ${ARGON2_LIBRARY}
           jwt-cpp::jwt-cpp
)

target_include_directories(
    ${PROJECT_NAME}_objs
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)

# Functional testing
userver_testsuite_add_simple()

# Install
include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
    message(STATUS "Set install prefix: $ENV{PREFIX}")
    file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
    set(CMAKE_INSTALL_PREFIX "${PREFIX_PATH}")
endif()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT ${PROJECT_NAME}
)
install(
    FILES ${CONFIGS_FILES}
    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}
    COMPONENT ${PROJECT_NAME}
)
