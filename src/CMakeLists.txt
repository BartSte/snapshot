add_subdirectory(helpers)
add_subdirectory(video)
add_subdirectory(gui)
add_subdirectory(app)

file(COPY static DESTINATION ${CMAKE_BINARY_DIR})
add_executable(snapshot main.cpp resources/resources.qrc)
target_link_libraries(snapshot PRIVATE app)
