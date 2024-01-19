set(Boost_USE_STATIC_LIBS ON)
find_package(spdlog REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets
                                     WaylandClient)
include(${CMAKE_SOURCE_DIR}/cmake/FindFFmpeg_patched.cmake)

get_target_property(QT6_LIB_TYPE Qt6::Widgets TYPE)
set(QT6_LIB_TYPE ${QT6_LIB_TYPE} PARENT_SCOPE)

get_target_property(FFMPEG_LIB_TYPE FFmpeg::FFmpeg TYPE)

message(STATUS "Qt6 was found at ${Qt6_DIR}")
message(STATUS "Qt6 library type is ${QT6_LIB_TYPE}")
message(STATUS "FFmpeg libraries are: ${FFMPEG_LIBRARIES}")
message(STATUS "FFmpeg library type is ${FFMPEG_LIB_TYPE}")

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*")
file(GLOB_RECURSE INCLUDES "${CMAKE_SOURCE_DIR}/include/*")

add_library(snapshotapp ${SOURCES} ${INCLUDES})
target_include_directories(
  snapshotapp
  PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/3rdparty/include
         ${Boost_INCLUDE_DIRS}
  INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/snapshotapp_autogen/include)

set(PLATFORM_PLUGINS
    Qt6::QEglFSIntegrationPlugin
    Qt6::QLinuxFbIntegrationPlugin
    Qt6::QMinimalEglIntegrationPlugin
    Qt6::QMinimalIntegrationPlugin
    Qt6::QOffscreenIntegrationPlugin
    Qt6::QVkKhrDisplayIntegrationPlugin
    Qt6::QVncIntegrationPlugin
    Qt6::QWaylandEglPlatformIntegrationPlugin
    Qt6::QWaylandIntegrationPlugin
    Qt6::QXcbIntegrationPlugin)
set(MULTIMEDIA_PLUGINS Qt6::QFFmpegMediaPlugin)

target_link_libraries(
  snapshotapp
  PUBLIC spdlog::spdlog_header_only
         Boost::boost
         Boost::filesystem
         Qt6::Widgets
         Qt6::Multimedia
         Qt6::MultimediaWidgets
         ${MULTIMEDIA_PLUGINS}
         ${PLATFORM_PLUGINS})

qt_import_plugins(
  snapshotapp
  INCLUDE_BY_TYPE
  platforms
  ${PLATFORM_PLUGINS}
  INCLUDE_BY_TYPE
  multimedia
  ${MULTIMEDIA_PLUGINS})
