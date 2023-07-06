cmake_minimum_required(VERSION 3.5)
project(
  snapshot
  VERSION 0.0.0
  LANGUAGES CXX)

# Variables
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")

# Path
include_directories(${CMAKE_SOURCE_DIR}/src)

# Dependencies
find_package(Qt6 REQUIRED COMPONENTS Widgets)
include(GoogleTest)
include(FetchContent)
FetchContent_Declare(
  googletest
  DOWNLOAD_EXTRACT_TIMESTAMP true
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Libs
add_library(gui src/mainwindow.cpp src/mainwindow.h src/mainwindow.ui
                src/videoscene.h src/videoscene.cpp)
target_link_libraries(gui Qt6::Widgets)

# Snapshot
add_executable(snapshot src/mainwindow.cpp src/mainwindow.h src/mainwindow.ui
                        src/main.cpp)
target_link_libraries(snapshot gui)

# Tests
add_executable(snapshot_test tests/main.cpp tests/viewer_test.cpp)
gtest_discover_tests(snapshot_test)
target_link_libraries(snapshot_test Qt6::Widgets GTest::gtest gui)
