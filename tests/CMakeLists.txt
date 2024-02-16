include(${CMAKE_SOURCE_DIR}/cmake/Install.cmake)

find_package(Qt6 REQUIRED COMPONENTS Test)
include(GoogleTest)
include(FetchContent)
FetchContent_Declare(
  googletest
  DOWNLOAD_EXTRACT_TIMESTAMP true
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

file(COPY static DESTINATION .)
file(GLOB_RECURSE TEST_SOURCES "*.cpp")
add_executable(tests_bin ${TEST_SOURCES})
target_link_libraries(tests_bin snapshot gtest gmock Qt6::Test)
add_test(NAME tests_bin COMMAND tests_bin)
set_tests_properties(tests_bin PROPERTIES ENVIRONMENT
                                          "QT_QPA_PLATFORM=offscreen")

install(PROGRAMS ${CMAKE_SOURCE_DIR}/scripts/run_tests DESTINATION .)
install(DIRECTORY static DESTINATION .)
install_with_runtime_dependencies(tests_bin)
