add_executable(snapshot_bin main.cpp
                            ${CMAKE_SOURCE_DIR}/resources/resources.qrc)
target_link_libraries(snapshot_bin PRIVATE snapshot)

install(PROGRAMS ${CMAKE_SOURCE_DIR}/scripts/run_snapshot DESTINATION .)
install(
  TARGETS snapshot_bin
  DESTINATION .
  RUNTIME_DEPENDENCIES DIRECTORIES ${FFMPEG_LIBRARY_DIRS} POST_EXCLUDE_REGEXES
  ${EXCLUDE_C_LIBS_REGEX})

# When Qt is built as a shared library, the platform plugins must be installed
# alongside the application in the platforms directory.
if(QT6_LIB_TYPE STREQUAL "SHARED_LIBRARY")

  # TODO: is it not better if I use the cmake targets to point to the plugins?
  # Like Qt6::WaylandIntegrationPlugin?
  install(
    DIRECTORY ${QT6_TOP_DIR}/plugins/egldeviceintegrations/
    DESTINATION egldeviceintegrations
    PATTERN "libqeglfs-.*")

  install(
    DIRECTORY ${QT6_TOP_DIR}/plugins/platforms/
    DESTINATION platforms
    PATTERN "libqwayland-.*"
    PATTERN "libqxcb.*"
    PATTERN "libqminimal.*"
    PATTERN "libqoffscreen.*")

  install(
    DIRECTORY ${QT6_TOP_DIR}/plugins/imageformats/
    DESTINATION imageformats
    PATTERN "libqjpeg\\..*")

  install(
    DIRECTORY ${QT6_TOP_DIR}/plugins/multimedia/
    DESTINATION multimedia
    PATTERN "libffmpegmediaplugin\\..*")

  install(
    DIRECTORY ${QT6_TOP_DIR}/plugins/tls/
    DESTINATION tls
    PATTERN "libqopenssl\\..*")

  install(
    DIRECTORY ${QT6_TOP_DIR}/plugins/wayland-graphics-integration-client/
    DESTINATION wayland-graphics-integration-client
    PATTERN "libqt-plugin-wayland-egl.*"
    PATTERN "libdmabuf-server\\..*"
    PATTERN "libdrm-egl-server\\..*"
    PATTERN "libqt-plugin-wayland-egl\\..*"
    PATTERN "libshm-emulation-server\\..*"
    PATTERN "libvulkan-server\\..*")

  install(
    DIRECTORY ${QT6_TOP_DIR}/plugins/wayland-shell-integration/
    DESTINATION wayland-shell-integration
    PATTERN ".*-shell\\..*"
    PATTERN ".*-shell-v1\\..*"
    PATTERN ".*-shell-plugin\\..*")

  install(
    DIRECTORY ${QT6_TOP_DIR}/plugins/xcbglintegrations/
    DESTINATION xcbglintegrations
    PATTERN "libqxcb-.*-integration\\.*")

endif()
