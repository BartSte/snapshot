include(${CMAKE_SOURCE_DIR}/cmake/Find.cmake)

set(Boost_USE_STATIC_LIBS ON)
find_package(spdlog REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

# Also finds FFmpeg libraries see FindFFmpegPatched.cmake for the variables
# that are created
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
  set(MULTIMEDIA_PLUGINS Qt6::QFFmpegMediaPlugin)

  find_libva() # creates Libva::va/va-drm/va-x11/va-wayland

  get_target_property(VA_LIBRARY Libva::va LOCATION)
  get_target_property(VA_DRM_LIBRARY Libva::va-drm LOCATION)
  get_target_property(VA_X11_LIBRARY Libva::va-x11 LOCATION)
  get_target_property(VA_WAYLAND_LIBRARY Libva::va-wayland LOCATION)

  message(STATUS "Libva::va library is: ${VA_LIBRARY}")
  message(STATUS "Libva::va-drm library is: ${VA_DRM_LIBRARY}")
  message(STATUS "Libva::va-x11 library is: ${VA_X11_LIBRARY}")
  message(STATUS "Libva::va-wayland library is: ${VA_WAYLAND_LIBRARY}")

  list(
    APPEND
    LINK_LIBS
    FFmpeg::FFmpeg
    Libva::va
    Libva::va-drm
    Libva::va-x11
    Libva::va-wayland
    ${PLATFORM_PLUGINS}
    ${MULTIMEDIA_PLUGINS})

  qt_import_plugins(
    snapshotapp
    INCLUDE_BY_TYPE
    platforms
    ${PLATFORM_PLUGINS}
    INCLUDE_BY_TYPE
    multimedia
    ${MULTIMEDIA_PLUGINS})
endif()

target_link_libraries(snapshotapp PUBLIC ${LINK_LIBS})
