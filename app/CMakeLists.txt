add_executable(snapshot_bin main.cpp ${CMAKE_SOURCE_DIR}/resources/resources.qrc)
target_link_libraries(snapshot_bin PRIVATE snapshot)

install(
  TARGETS snapshot_bin
  DESTINATION .
  RUNTIME_DEPENDENCIES
  DIRECTORIES
  ${FFMPEG_LIBRARY_DIRS}
  POST_EXCLUDE_REGEXES
  # Compatability with standard C libraries across linux distributions is
  # assumed.
  ".*ld-linux.*\\.so.*"
  ".*libc\\.so.*"
  ".*libm\\.so.*"
  ".*libdbm\\.so.*"
  ".*libpthread\\.so.*")

# When Qt is built as a shared library, the platform plugins must be installed
# alongside the application in the platforms directory.
if(QT6_LIB_TYPE STREQUAL "SHARED_LIBRARY")

  install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/3rdparty/Qt/plugins/egldeviceintegrations/
    DESTINATION egldeviceintegrations
    PATTERN "libqeglfs-.*")

  install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/3rdparty/Qt/plugins/platforms/
    DESTINATION platforms
    PATTERN "libqwayland-.*"
    PATTERN "libqxcb.*"
    PATTERN "libqminimal.*"
    PATTERN "libqoffscreen.*")

  install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/3rdparty/Qt/plugins/imageformats/
    DESTINATION imageformats
    PATTERN "libqjpeg\\..*")

  install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/3rdparty/Qt/plugins/multimedia/
    DESTINATION multimedia
    PATTERN "libffmpegmediaplugin\\..*")

  install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/3rdparty/Qt/plugins/tls/
    DESTINATION tls
    PATTERN "libqopenssl\\..*")

  install(
    DIRECTORY
      ${CMAKE_SOURCE_DIR}/3rdparty/Qt/plugins/wayland-graphics-integration-client/
    DESTINATION wayland-graphics-integration-client
    PATTERN "libqt-plugin-wayland-egl.*"
    PATTERN "libdmabuf-server\\..*"
    PATTERN "libdrm-egl-server\\..*"
    PATTERN "libqt-plugin-wayland-egl\\..*"
    PATTERN "libshm-emulation-server\\..*"
    PATTERN "libvulkan-server\\..*")

  install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/3rdparty/Qt/plugins/wayland-shell-integration/
    DESTINATION wayland-shell-integration
    PATTERN ".*-shell\\..*"
    PATTERN ".*-shell-v1\\..*"
    PATTERN ".*-shell-plugin\\..*")

  install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/3rdparty/Qt/plugins/xcbglintegrations/
    DESTINATION xcbglintegrations
    PATTERN "libqxcb-.*-integration\\.*")

endif()

# Copy the entrypoint to the install directory.
install(PROGRAMS ${CMAKE_SOURCE_DIR}/scripts/run_snapshot DESTINATION .)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION_FULL})
set(CPACK_GENERATOR "TGZ")

include(CPack)
