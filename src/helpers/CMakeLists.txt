project(helpers)

find_package(spdlog REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

file(GLOB_RECURSE SOURCES ./*.cpp)
file(GLOB_RECURSE HEADERS ./*.hpp)

add_library(helpers ${SOURCES} ${HEADERS})
target_include_directories(
  helpers
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ 
  PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(helpers PRIVATE spdlog::spdlog_header_only Qt6::Widgets
                                      Boost::filesystem)
