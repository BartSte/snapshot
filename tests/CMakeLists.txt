find_package(Qt6 REQUIRED COMPONENTS Test)
include(GoogleTest)
include(FetchContent)
FetchContent_Declare(
  googletest
  DOWNLOAD_EXTRACT_TIMESTAMP true
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

file(COPY static DESTINATION ${CMAKE_BINARY_DIR})
file(GLOB_RECURSE TEST_SOURCES "*.cpp")
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests snapshot gtest gmock Qt6::Test)
add_test(NAME tests COMMAND tests)
set_tests_properties(tests PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
