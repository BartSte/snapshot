cmake_minimum_required(VERSION 3.5)

project(snapshot VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5untitled REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable( 
    snapshot
    main.cpp
    app.cpp
    app.h
    mainwindow.h
    viewer.h
    viewer.cpp
    )

target_link_libraries(snapshot Qt6::Widgets)
