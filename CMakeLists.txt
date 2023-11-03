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
find_package(spdlog REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options filesystem)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

# Add directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Helpers
add_library(
  helpers
  src/helpers/argparse.cpp src/helpers/argparse.hpp src/helpers/config.cpp
  src/helpers/config.hpp src/helpers/logger.cpp src/helpers/logger.hpp)
target_include_directories(
  helpers INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/gui_autogen/include
                    ${Boost_INCLUDE_DIRS}})
target_link_libraries(helpers ${Boost_LIBRARIES} spdlog::spdlog_header_only)

# video
add_library(video src/video/find.cpp src/video/find.hpp
                   src/video/connect.cpp src/video/connect.hpp)
target_include_directories(video INTERFACE ${Boost_INCLUDE_DIRS}})
target_link_libraries(video Qt6::Widgets Qt6::Multimedia ${Boost_LIBRARIES}
                      spdlog::spdlog_header_only)

# GUI
add_library(gui src/gui/mainwindow.cpp src/gui/mainwindow.ui
                src/gui/videoscene.cpp src/gui/videoscene.hpp)
target_include_directories(
  gui INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/gui_autogen/include
                ${Boost_INCLUDE_DIRS}})
target_link_libraries(
  gui
  video
  Qt6::Widgets
  Qt6::Multimedia
  Qt6::MultimediaWidgets
  ${Boost_LIBRARIES}
  spdlog::spdlog_header_only)

# Executable
file(COPY ${CMAKE_SOURCE_DIR}/src/static
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_executable(
  snapshot src/main.cpp src/snapshot/app.cpp src/snapshot/app.hpp
           src/snapshot/gui.cpp src/snapshot/gui.hpp resources/resources.qrc)
target_link_libraries(snapshot gui helpers)

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
  tests/main.cpp tests/ resources/resources.qrc tests/video_test/find_test.cpp
  tests/helpers_test/argparse_test.cpp tests/helpers_test/config_test.cpp)
target_link_libraries(tests gtest gmock video gui helpers)
