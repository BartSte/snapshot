add_executable(snapshot main.cpp ${CMAKE_SOURCE_DIR}/resources/resources.qrc)
target_link_libraries(snapshot PRIVATE snapshotapp)

install(
  TARGETS snapshot
          RUNTIME_DEPENDENCIES
          POST_EXCLUDE_REGEXES
          # Compatability with standard C libraries across linux distributions
          # is assumed.
          ".*libc\.so.*"
          ".*libm\.so.*"
          ".*libdbm\.so.*"
          ".*libpthread\.so.*"
          # The FFmpeg libraries are added explicitly below to avoid conflicts
          # libs that are installed elsewhere on the system
          ".*libswscale\.so.*"
          ".*libavutil\.so.*"
          ".*libavcodec\.so.*"
          ".*libavformat\.so.*"
          ".*libswresample\.so.*"
          POST_INCLUDE_FILES
          # FFmpeg libraries are added explicitly below to avoid conflicts
          ${FFMPEG_LIBRARIES})

# Copy the entrypoint to the install directory.
install(PROGRAMS ${CMAKE_SOURCE_DIR}/scripts/snapshot DESTINATION .)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "TGZ")

include(CPack)
