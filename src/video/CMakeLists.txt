find_package(spdlog REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets Test)

file(GLOB_RECURSE SOURCES ./*.cpp)
file(GLOB_RECURSE HEADERS ./*.hpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(video ${SOURCES} ${HEADERS})
target_link_libraries(video helpers spdlog::spdlog_header_only Qt6::Widgets
                      Qt6::Multimedia)

