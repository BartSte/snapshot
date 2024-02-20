include(${CMAKE_SOURCE_DIR}/cmake/Install.cmake)

add_executable(snapshot_bin main.cpp
                            ${CMAKE_SOURCE_DIR}/resources/resources.qrc)
target_link_libraries(snapshot_bin PRIVATE snapshot)

install(PROGRAMS ${INSTALL_SCRIPTS}/snapshot DESTINATION .)
install_with_runtime_dependencies(snapshot_bin)

if(QT6_LIB_TYPE STREQUAL "SHARED_LIBRARY")
  install_qt_shared_plugins()
endif()
