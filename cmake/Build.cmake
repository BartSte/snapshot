include(${CMAKE_SOURCE_DIR}/cmake/Find.cmake)

# cmake-format: off
# When building the project with a static Qt6 library, we need to link the
# Libva library to the Qt6 FFmpeg media plugin. This is necessary because
# otherwise some symbols are not found when linking the snapshotapp.
# cmake-format: on
macro(link_libva_to_ffmpeg_plugin)
  find_libva() # creates Libva::va/va-drm/va-x11/va-wayland
  get_target_property(QT6_QFFMPEGMEDIAPLUGIN_INTERFACE_LINK_LIBRARIES
                      Qt6::QFFmpegMediaPlugin INTERFACE_LINK_LIBRARIES)
  list(APPEND QT6_QFFMPEGMEDIAPLUGIN_INTERFACE_LINK_LIBRARIES Libva::va
       Libva::va-drm Libva::va-x11 Libva::va-wayland)
  set_target_properties(
    Qt6::QFFmpegMediaPlugin
    PROPERTIES INTERFACE_LINK_LIBRARIES
               "${QT6_QFFMPEGMEDIAPLUGIN_INTERFACE_LINK_LIBRARIES}")
endmacro()

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
      Qt6::QMinimalEglIntegrationPlugin
      Qt6::QMinimalIntegrationPlugin
      Qt6::QOffscreenIntegrationPlugin
      Qt6::QVkKhrDisplayIntegrationPlugin
      Qt6::QVncIntegrationPlugin
      Qt6::QWaylandEglPlatformIntegrationPlugin
      Qt6::QWaylandIntegrationPlugin
      Qt6::QXcbIntegrationPlugin)

  link_libva_to_ffmpeg_plugin()

  # Plugins are not linked automatically when building with a static Qt6
  # library, so we need to link them manually.
  list(APPEND LINK_LIBS FFmpeg::FFmpeg Qt6::QFFmpegMediaPlugin
       ${PLATFORM_PLUGINS})
  target_link_libraries(snapshotapp PUBLIC ${LINK_LIBS})

  # Import the Qt plugins in the code by setting macros.
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
endmacro()
