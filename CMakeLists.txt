include(${CMAKE_SOURCE_DIR}/cmake/Version.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/Install.cmake)

cmake_minimum_required(VERSION 3.5)
project(snapshot LANGUAGES CXX)
set_git_tag_as_project_version()
set(PROJECT_VERSION ${PROJECT_VERSION_TAG})

option(BUILD_TESTING "Build tests" OFF)
option(INSTALL_GTEST OFF)

message(STATUS "Project version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build tests: ${BUILD_TESTING}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INSTALL_SCRIPTS
    ${CMAKE_SOURCE_DIR}/scripts/install
    CACHE PATH "Install scripts")
set(CMAKE_INSTALL_PREFIX /opt/${PROJECT_NAME})
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")

configure_file(${CMAKE_SOURCE_DIR}/include/version.h.in
               ${CMAKE_SOURCE_DIR}/include/version.h @ONLY)

add_subdirectory(src)
add_subdirectory(app)

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION_FULL})
set(CPACK_GENERATOR "TGZ")
include(CPack)
