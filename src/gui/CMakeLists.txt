set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(spdlog REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets Test)

file(GLOB_RECURSE SOURCES ./*.cpp)
file(GLOB_RECURSE HEADERS ./*.hpp)

add_library(gui ${SOURCES} ${HEADERS})
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
