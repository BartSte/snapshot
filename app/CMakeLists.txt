add_executable(snapshot main.cpp ${CMAKE_SOURCE_DIR}/resources/resources.qrc)
target_link_libraries(snapshot PRIVATE snapshotapp)

message(
  STATUS
    "Adding directory of FFmpeg libraries to the install search path: ${FFMPEG_LIBRARY_DIRS}"
)

install(
  TARGETS snapshot
          RUNTIME_DEPENDENCIES
          DIRECTORIES
          ${FFMPEG_LIBRARY_DIRS}
          POST_EXCLUDE_REGEXES
          # Compatability with standard C libraries across linux distributions
          # is assumed.
          ".*ld-linux.*\\.so.*"
          ".*libc\\.so.*"
          ".*libm\\.so.*"
          ".*libdbm\\.so.*"
          ".*libpthread\\.so.*")

# Copy the entrypoint to the install directory.
install(PROGRAMS ${CMAKE_SOURCE_DIR}/scripts/snapshot DESTINATION .)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "TGZ")

include(CPack)
