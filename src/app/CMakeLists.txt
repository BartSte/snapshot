find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

add_library(app ./snapshotapp.cpp ./snapshotapp.hpp)
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(app PUBLIC gui video helpers Qt6::Widgets Qt6::Multimedia
                                 Qt6::MultimediaWidgets)
