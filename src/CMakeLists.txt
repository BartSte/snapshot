add_library(
    LibSnapshot
    mainwindow.cpp
    mainwindow.h
    viewer.h
    viewer.cpp
    mainwindow.ui)

add_executable( 
    snapshot 
    main.cpp)

target_link_libraries(LibSnapshot Qt6::Widgets)
target_link_libraries(snapshot LibSnapshot)
