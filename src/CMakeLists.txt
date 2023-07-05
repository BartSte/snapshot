add_library(
    LibSnapshot
    mainwindow.cpp
    mainwindow.h
    videoscene.h
    videoscene.cpp
    mainwindow.ui)

add_executable( 
    snapshot 
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui    
    main.cpp)

target_link_libraries(LibSnapshot Qt6::Widgets)
target_link_libraries(snapshot LibSnapshot)
