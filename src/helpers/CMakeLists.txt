find_package(spdlog REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets Test)

file(GLOB_RECURSE SOURCES ./*.cpp)
file(GLOB_RECURSE HEADERS ./*.hpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(helpers ${SOURCES} ${HEADERS})
target_link_libraries(helpers spdlog::spdlog_header_only Qt6::Widgets
                      Boost::filesystem)
