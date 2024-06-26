include(${CMAKE_SOURCE_DIR}/cmake/Find.cmake)

# cmake-format: off
# When building the project with a static Qt6 library, we need to link the
# following components statically as well: 
# - Platform plugins 
# - Multimedia plugins 
# - FFmpeg 
# - Libva 
# Furthermore, the Qt plugins need to be imported manually using the
# qt_import_plugins() function.
# cmake-format: on
macro(target_link_libraries_static_qt)
  set(PLATFORM_PLUGINS
      Qt6::QEglFSIntegrationPlugin
      Qt6::QLinuxFbIntegrationPlugin
      Qt6::QOffscreenIntegrationPlugin
      Qt6::QVkKhrDisplayIntegrationPlugin
      Qt6::QWaylandEglPlatformIntegrationPlugin
      Qt6::QWaylandIntegrationPlugin
      Qt6::QXcbIntegrationPlugin)

  set(MULTIMEDIA_PLUGINS Qt6::QFFmpegMediaPlugin)

  link_libva_to_ffmpeg_plugin()

  # Plugins are not linked automatically when building with a static Qt6
  # library, so we need to link them manually.
  list(APPEND LINK_LIBS FFmpeg::FFmpeg ${MULTIMEDIA_PLUGINS}
       ${PLATFORM_PLUGINS})
  target_link_libraries(snapshot PUBLIC ${LINK_LIBS})

  # Import the Qt plugins in the code by setting macros.
  qt_import_plugins(
    snapshot
    INCLUDE_BY_TYPE
    platforms
    ${PLATFORM_PLUGINS}
    INCLUDE_BY_TYPE
    multimedia
    ${MULTIMEDIA_PLUGINS}
    EXCLUDE
    Qt6::QGstreamerMediaPlugin
    NO_DEFAULT)
endmacro()

# cmake-format: off
# When building the project with a static Qt6 library, we need to link the
# Libva library to the Qt6 FFmpeg media plugin. This is necessary because
# otherwise some symbols are not found when linking the snapshot.
# cmake-format: on
macro(link_libva_to_ffmpeg_plugin)
  find_libva() # creates Libva::va/va-drm/va-x11/va-wayland
  get_target_property(QT6_QFFMPEGMEDIAPLUGIN_INTERFACE_LINK_LIBRARIES
                      ${MULTIMEDIA_PLUGINS} INTERFACE_LINK_LIBRARIES)
  list(APPEND QT6_QFFMPEGMEDIAPLUGIN_INTERFACE_LINK_LIBRARIES Libva::va
       Libva::va-drm Libva::va-x11 Libva::va-wayland)
  set_target_properties(
    ${MULTIMEDIA_PLUGINS}
    PROPERTIES INTERFACE_LINK_LIBRARIES
               "${QT6_QFFMPEGMEDIAPLUGIN_INTERFACE_LINK_LIBRARIES}")
endmacro()
