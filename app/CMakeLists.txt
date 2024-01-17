add_executable(snapshot main.cpp ${CMAKE_SOURCE_DIR}/resources/resources.qrc)
target_link_libraries(snapshot PRIVATE snapshotapp)

install(
  TARGETS snapshot
          RUNTIME_DEPENDENCIES
          POST_EXCLUDE_REGEXES
          ".*libc.*"
          ".*libm\..*"
          ".*libavcodec.*"
          ".*libavdevice.*"
          ".*libavfilter.*"
          ".*libavformat.*"
          ".*libavutil.*"
          ".*libpostproc.*"
          ".*libswresample.*"
          ".*libswscale.*"
          ".*libvmaf.*")
install(FILES ${FFMPEG_LIBRARIES} DESTINATION lib)
install(PROGRAMS ${CMAKE_SOURCE_DIR}/scripts/snapshot DESTINATION .)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "TGZ")

include(CPack)
