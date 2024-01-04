add_executable(snapshot main.cpp ${CMAKE_SOURCE_DIR}/resources/resources.qrc)
target_link_libraries(snapshot PRIVATE snapshotapp)
install(TARGETS snapshot DESTINATION bin)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "ZIP")

include(CPack)
