# When Qt is built as a shared library, the platform plugins must be installed
# alongside the application in the directories that have the same name as in the
# qt source.
macro(install_qt_shared_plugins)

  # Natually, we need X11 and Wayland support. Eglfs and Linuxfb are also needed
  # as a Raspberry Pi may require them. Offscreen is usefull when you do not
  # have a display available. Lets also support vulkan.
  install(
    DIRECTORY ${QT6_TOP_DIR}/plugins/platforms/
    DESTINATION platforms
    PATTERN "libqeglfs-.*"
    PATTERN "libqlinuxfb.*"
    PATTERN "libqoffscreen.*"
    PATTERN "libqvkkhrdisplay.*"
    PATTERN "libqwayland-.*"
    PATTERN "libqxcb.*")

  # Support both egl and glx.
  install(
    DIRECTORY ${QT6_TOP_DIR}/plugins/xcbglintegrations/
    DESTINATION xcbglintegrations
    PATTERN "libqxcb-.*-integration\\.*")

  # Support all the wayland graphics integrations.
  install(
    DIRECTORY ${QT6_TOP_DIR}/plugins/wayland-graphics-integration-client/
    DESTINATION wayland-graphics-integration-client
    PATTERN "libdmabuf-server\\..*"
    PATTERN "libdrm-egl-server\\..*"
    PATTERN "libqt-plugin-wayland-egl\\..*"
    PATTERN "libshm-emulation-server\\..*"
    PATTERN "libvulkan-server\\..*")

  # X11 support for eglfs can be used by Raspberry Pi.
  install(
    DIRECTORY ${QT6_TOP_DIR}/plugins/egldeviceintegrations/
    DESTINATION egldeviceintegrations
    PATTERN "libqeglfs-x11-.*")

  # Support all shells
  install(
    DIRECTORY ${QT6_TOP_DIR}/plugins/wayland-shell-integration/
    DESTINATION wayland-shell-integration
    PATTERN ".*-shell\\..*"
    PATTERN ".*-shell-v1\\..*"
    PATTERN ".*-shell-plugin\\..*")

  # Only saving as jpeg is supported.
  install(
    DIRECTORY ${QT6_TOP_DIR}/plugins/imageformats/
    DESTINATION imageformats
    PATTERN "libqjpeg\\..*")

  # Only ffmpeg is supported.
  install(
    DIRECTORY ${QT6_TOP_DIR}/plugins/multimedia/
    DESTINATION multimedia
    PATTERN "libffmpegmediaplugin\\..*")

endmacro()
