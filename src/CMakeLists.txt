include(${CMAKE_SOURCE_DIR}/cmake/Find.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/Build.cmake)

set(Boost_USE_STATIC_LIBS ON)
find_package(spdlog REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

# Also finds FFmpeg libraries see FindFFmpegPatched.cmake for the variables that
# are created
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets
                                     WaylandClient)
get_target_property(QT6_LIB_TYPE Qt6::Widgets TYPE)
set(QT6_LIB_TYPE
    ${QT6_LIB_TYPE}
    PARENT_SCOPE)
message(STATUS "Qt6 was found at ${Qt6_DIR}")
message(STATUS "Qt6 library type is ${QT6_LIB_TYPE}")
message(STATUS "FFmpeg libraries are: ${FFMPEG_LIBRARIES}")

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*")
file(GLOB_RECURSE INCLUDES "${CMAKE_SOURCE_DIR}/include/*")

add_library(snapshot ${SOURCES} ${INCLUDES})
target_include_directories(
  snapshot
  PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/3rdparty/include
         ${Boost_INCLUDE_DIRS}
  INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/snapshot_autogen/include)

set(LINK_LIBS spdlog::spdlog_header_only Boost::boost Boost::filesystem
              Qt6::Widgets Qt6::Multimedia Qt6::MultimediaWidgets)

if(QT6_LIB_TYPE STREQUAL "STATIC_LIBRARY")
  target_link_libraries_static_qt()
else()
  target_link_libraries(snapshot PUBLIC ${LINK_LIBS})
endif()
