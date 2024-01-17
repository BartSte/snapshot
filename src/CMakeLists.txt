set(Boost_USE_STATIC_LIBS ON)
find_package(spdlog REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets
                                     WaylandClient)

message(STATUS "Qt6 was found at ${Qt6_DIR}")

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

target_link_libraries(
  snapshotapp
  PUBLIC spdlog::spdlog_header_only
         Boost::boost
         Boost::filesystem
         Qt6::Widgets
         Qt6::Multimedia
         Qt6::MultimediaWidgets
         Qt6::QFFmpegMediaPlugin
         ${PLATFORM_PLUGINS})

qt_import_plugins(
  snapshotapp
  INCLUDE_BY_TYPE
  platforms
  ${PLATFORM_PLUGINS}
  INCLUDE_BY_TYPE
  multimedia
  Qt6::QFFmpegMediaPlugin)
