cmake_minimum_required(VERSION 3.5)
project(
  snapshot
  VERSION 0.0.0
  LANGUAGES CXX)

# Variables
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

# Find packages
find_package(cxxopts REQUIRED)
find_package(spdlog REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets Test)

# Add directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIRS})

# Helpers
add_library(
  helpers
  src/helpers/argparse.cpp
  src/helpers/argparse.hpp
  src/helpers/config.cpp
  src/helpers/config.hpp
  src/helpers/time.cpp
  src/helpers/time.hpp
  src/helpers/path.cpp
  src/helpers/path.hpp)
target_link_libraries(helpers spdlog::spdlog_header_only Qt6::Widgets
                      Boost::filesystem)

# Video
add_library(
  video src/video/find.cpp src/video/find.hpp src/video/connect.cpp
        src/video/connect.hpp src/video/record.cpp src/video/record.hpp)
target_link_libraries(video helpers spdlog::spdlog_header_only Qt6::Widgets
                      Qt6::Multimedia)

# GUI
add_library(
  gui src/gui/mainwindow.cpp src/gui/mainwindow.ui src/gui/videoscene.cpp
      src/gui/videoscene.hpp)
target_include_directories(
  gui INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/gui_autogen/include)
target_link_libraries(
  gui
  video
  helpers
  spdlog::spdlog_header_only
  Qt6::Widgets
  Qt6::Multimedia
  Qt6::MultimediaWidgets)

# Snapshot
file(COPY ${CMAKE_SOURCE_DIR}/src/static
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_library(app src/app/app.cpp src/app/app.hpp)
target_link_libraries(app gui video helpers)

# Executable
add_executable(snapshot src/main.cpp resources/resources.qrc)
target_link_libraries(snapshot app)

# Tests
file(COPY ${CMAKE_SOURCE_DIR}/tests/static
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
include(GoogleTest)
include(FetchContent)
FetchContent_Declare(
  googletest
  DOWNLOAD_EXTRACT_TIMESTAMP true
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
add_executable(
  tests
  tests/main.cpp
  tests/test_app/test_app.cpp
  tests/test_video/test_find.cpp
  tests/test_video/test_connect.cpp
  tests/test_video/test_record.cpp
  tests/test_helpers/test_argparse.cpp
  tests/test_helpers/test_config.cpp
  tests/test_helpers/test_time.cpp)
target_link_libraries(
  tests
  app
  gtest
  gmock
  Qt6::Test)
