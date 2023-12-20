project(video)

find_package(spdlog REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

file(GLOB_RECURSE SOURCES ./*.cpp)
file(GLOB_RECURSE HEADERS ./*.hpp)

add_library(video ${SOURCES} ${HEADERS})
target_include_directories(video PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_link_libraries(video PRIVATE helpers spdlog::spdlog_header_only
                                    Qt6::Widgets Qt6::Multimedia)
