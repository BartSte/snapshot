cmake_minimum_required(VERSION 3.5)
project(
  snapshot
  VERSION 0.0.0
  LANGUAGES CXX)

# Variables
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")

# Gui
find_package(spdlog REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)
add_library(
  gui
  src/mainwindow.cpp
  src/mainwindow.ui
  src/videoscene.h
  src/videoscene.cpp
  src/argparse.cpp
  src/argparse.h
  src/logger.h
  src/logger.cpp)
target_include_directories(
  gui INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/gui_autogen/include
                ${Boost_INCLUDE_DIRS}})
target_link_libraries(gui Qt6::Widgets Qt6::Multimedia Qt6::MultimediaWidgets
                      ${Boost_LIBRARIES} spdlog::spdlog_header_only)

# Snapshot
add_executable(snapshot src/main.cpp resources/resources.qrc)
target_link_libraries(snapshot gui)

# Tests
include_directories(${CMAKE_SOURCE_DIR}/src)
include(GoogleTest)
include(FetchContent)
FetchContent_Declare(
  googletest
  DOWNLOAD_EXTRACT_TIMESTAMP true
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
add_executable(tests tests/main.cpp tests/argparse_test.cpp)
target_link_libraries(tests gtest gui)
