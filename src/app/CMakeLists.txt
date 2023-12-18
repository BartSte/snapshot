set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

add_library(app ./app.cpp ./app.hpp)
target_link_libraries(app gui video helpers)
