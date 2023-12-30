list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/3rdparty/Qt/6.6.1/gcc_64/lib/cmake)

find_package(spdlog REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

message(STATUS "Qt6 was found at ${Qt6_DIR}")

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*")
file(GLOB_RECURSE INCLUDES "${CMAKE_SOURCE_DIR}/include/*")

add_library(snapshotapp ${SOURCES} ${INCLUDES})
target_include_directories(
  snapshotapp
  PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/3rdparty/include ${Boost_INCLUDE_DIRS}
  INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/snapshotapp_autogen/include)
target_link_libraries(
  snapshotapp PUBLIC spdlog::spdlog_header_only Qt6::Widgets Qt6::Multimedia
                     Qt6::MultimediaWidgets Boost::filesystem)
install(TARGETS snapshotapp DESTINATION lib)
