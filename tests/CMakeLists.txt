include_directories(${CMAKE_SOURCE_DIR}/src)

include(FetchContent)
FetchContent_Declare(
  googletest
  DOWNLOAD_EXTRACT_TIMESTAMP true
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(snapshot_test main.cpp viewer_test.cpp)

target_link_libraries(snapshot_test Qt6::Widgets)
target_link_libraries(snapshot_test GTest::gtest)
target_link_libraries(snapshot_test LibSnapshot)

include(GoogleTest)
gtest_discover_tests(snapshot_test)
