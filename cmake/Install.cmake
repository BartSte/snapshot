function(install_with_runtime_dependencies binary)
  # Compatability with the following C libraries across linux distributions is
  # assumed. Use this constant in the install command to exclude these libraries
  # from the installation.
  set(exclude_c_libs_regex
      ".*ld-linux.*\\.so.*" ".*libc\\.so.*" ".*libm\\.so.*" ".*libdbm\\.so.*"
      ".*libpthread\\.so.*")

  install(
    TARGETS ${binary}
    DESTINATION .
    RUNTIME_DEPENDENCIES DIRECTORIES ${FFMPEG_LIBRARY_DIRS}
    POST_EXCLUDE_REGEXES ${exclude_c_libs_regex})
endfunction()

# When Qt is built as a shared library, the platform plugins must be installed
# alongside the application in the directories that have the same name as in the
# qt source.
macro(install_qt_shared_plugins)
  set(_plugin_dir ${QT6_TOP_DIR}/plugins)
  # Natually, we need X11 and Wayland support. Eglfs and Linuxfb are also needed
  # as a Raspberry Pi may require them. Offscreen is usefull when you do not
  # have a display available. Lets also support vulkan.
  install(
    FILES ${_plugin_dir}/platforms/libqeglfs.so
          ${_plugin_dir}/platforms/libqlinuxfb.so
          ${_plugin_dir}/platforms/libqoffscreen.so
          ${_plugin_dir}/platforms/libqvkkhrdisplay.so
          ${_plugin_dir}/platforms/libqwayland-egl.so
          ${_plugin_dir}/platforms/libqwayland-generic.so
          ${_plugin_dir}/platforms/libqxcb.so
    DESTINATION platforms)

  # Support all the wayland graphics integrations, except emulation.
  install(
    FILES
      ${_plugin_dir}/wayland-graphics-integration-client/libdmabuf-server.so
      ${_plugin_dir}/wayland-graphics-integration-client/libdrm-egl-server.so
      ${_plugin_dir}/wayland-graphics-integration-client/libqt-plugin-wayland-egl.so
      ${_plugin_dir}/wayland-graphics-integration-client/libvulkan-server.so
    DESTINATION wayland-graphics-integration-client)

  # X11 support for eglfs can be used by Raspberry Pi.
  install(
    FILES ${_plugin_dir}/egldeviceintegrations/libqeglfs-x11-integration.so
    DESTINATION egldeviceintegrations)

  # Jpeg is needded for image support.
  install(FILES ${_plugin_dir}/imageformats/libqjpeg.so
          DESTINATION imageformats)

  # Only ffmpeg is supported.
  install(FILES ${_plugin_dir}/multimedia/libffmpegmediaplugin.so
          DESTINATION multimedia)

  # Support both egl and glx.
  install(DIRECTORY ${_plugin_dir}/xcbglintegrations/
          DESTINATION xcbglintegrations)

  # Support all shells
  install(DIRECTORY ${_plugin_dir}/wayland-shell-integration/
          DESTINATION wayland-shell-integration)
endmacro()
