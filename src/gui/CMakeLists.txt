find_package(spdlog REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

file(GLOB_RECURSE SOURCES ./*.cpp)
file(GLOB_RECURSE HEADERS ./*.hpp)

add_library(gui ${SOURCES} ${HEADERS})
target_include_directories(
  gui
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../
  INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/gui_autogen/include)
target_link_libraries(
  gui PRIVATE video helpers spdlog::spdlog_header_only Qt6::Widgets
              Qt6::Multimedia Qt6::MultimediaWidgets)
