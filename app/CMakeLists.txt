file(COPY static DESTINATION ${CMAKE_BINARY_DIR})
add_executable(snapshot main.cpp resources/resources.qrc)
target_link_libraries(snapshot PRIVATE snapshotapp)
