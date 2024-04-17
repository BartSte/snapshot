include(${CMAKE_SOURCE_DIR}/cmake/Find.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/Build.cmake)

set(Boost_USE_STATIC_LIBS ON)
find_package(spdlog REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_qt6_base_and_multimedia() # Sets QT6_LIB_TYPE and QT6_TOP_DIR

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*")
file(GLOB_RECURSE INCLUDES "${CMAKE_SOURCE_DIR}/include/*")

add_library(snapshot ${SOURCES} ${INCLUDES})
target_include_directories(
  snapshot
  PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/3rdparty/include
         ${Boost_INCLUDE_DIRS}
  INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/snapshot_autogen/include)

set(LINK_LIBS spdlog::spdlog_header_only Boost::boost Boost::filesystem
              Qt6::Widgets Qt6::Multimedia Qt6::MultimediaWidgets)

if(QT6_LIB_TYPE STREQUAL "STATIC_LIBRARY")
  target_link_libraries_static_qt() # macro
else()
  # Avoids logging a warning, FFMPEG_DIR only needed for static linking
  set(FFMPEG_DIR _)
  target_link_libraries(snapshot PUBLIC ${LINK_LIBS})
endif()
