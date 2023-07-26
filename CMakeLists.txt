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

# Gui TODO: split the GUI from the non-gui (e.g. list)
file(COPY ${CMAKE_SOURCE_DIR}/src/static
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
find_package(spdlog REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options filesystem)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)
add_library(
  gui
  src/camera.hpp
  src/argparse.cpp
  src/argparse.hpp
  src/camera.cpp
  src/list.cpp
  src/list.hpp
  src/logger.cpp
  src/logger.hpp
  src/mainwindow.cpp
  src/mainwindow.ui
  src/videoscene.cpp
  src/videoscene.hpp
  src/config.cpp
  src/config.hpp)
target_include_directories(
  gui INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/gui_autogen/include
                ${Boost_INCLUDE_DIRS}})
target_link_libraries(gui Qt6::Widgets Qt6::Multimedia Qt6::MultimediaWidgets
                      ${Boost_LIBRARIES} spdlog::spdlog_header_only)

# Snapshot
add_executable(snapshot src/main.cpp resources/resources.qrc)
target_link_libraries(snapshot gui)

# Tests
file(COPY ${CMAKE_SOURCE_DIR}/tests/static
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
include(GoogleTest)
include(FetchContent)
FetchContent_Declare(
  googletest
  DOWNLOAD_EXTRACT_TIMESTAMP true
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
add_executable(tests tests/main.cpp tests/argparse_test.cpp tests/list_test.cpp
                     tests/config_test.cpp resources/resources.qrc)
target_link_libraries(tests gtest gui)
