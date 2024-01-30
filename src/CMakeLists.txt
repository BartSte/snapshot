include(${CMAKE_SOURCE_DIR}/cmake/Find.cmake)

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

add_library(snapshotapp ${SOURCES} ${INCLUDES})
target_include_directories(
  snapshotapp
  PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/3rdparty/include
         ${Boost_INCLUDE_DIRS}
  INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/snapshotapp_autogen/include)

set(LINK_LIBS spdlog::spdlog_header_only Boost::boost Boost::filesystem
              Qt6::Widgets Qt6::Multimedia Qt6::MultimediaWidgets)

# When building the project with a static Qt6 library, we need to link the
# following components statically as well: - Platform plugins - Multimedia
# plugins - FFmpeg - Libva Furthermore, the Qt plugins need to be imported
# manually using the qt_import_plugins() function.
if(QT6_LIB_TYPE STREQUAL "STATIC_LIBRARY")
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

  find_libva() # creates Libva::va/va-drm/va-x11/va-wayland
  get_target_property(QT6_QFFMPEGMEDIAPLUGIN_INTERFACE_LINK_LIBRARIES
                      Qt6::QFFmpegMediaPlugin INTERFACE_LINK_LIBRARIES)
  list(APPEND QT6_QFFMPEGMEDIAPLUGIN_INTERFACE_LINK_LIBRARIES Libva::va
       Libva::va-drm Libva::va-x11 Libva::va-wayland)
  set_target_properties(
    Qt6::QFFmpegMediaPlugin
    PROPERTIES INTERFACE_LINK_LIBRARIES
               "${QT6_QFFMPEGMEDIAPLUGIN_INTERFACE_LINK_LIBRARIES}")

  list(APPEND LINK_LIBS FFmpeg::FFmpeg Qt6::QFFmpegMediaPlugin ${PLATFORM_PLUGINS})
endif()

# CMAKE_DL_LIBS is needed for building on Ubuntu 20.04
target_link_libraries(snapshotapp PUBLIC ${LINK_LIBS} ${CMAKE_DL_LIBS})

if(QT6_LIB_TYPE STREQUAL "STATIC_LIBRARY")
  qt_import_plugins(
    snapshotapp
    INCLUDE_BY_TYPE
    platforms
    ${PLATFORM_PLUGINS}
    INCLUDE_BY_TYPE
    multimedia
    Qt6::QFFmpegMediaPlugin
    EXCLUDE
    Qt6::QGstreamerMediaPlugin
    NO_DEFAULT)
endif()
