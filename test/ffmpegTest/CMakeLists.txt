FILE(GLOB_RECURSE TEST_UNIT_SOURCE_FILES *.cpp)
FILE(GLOB_RECURSE TEST_UNIT_HEADER_FILES *.h)

add_executable(ffmpegTest ${TEST_UNIT_SOURCE_FILES} ${TEST_UNIT_HEADER_FILES})

target_include_directories(ffmpegTest PRIVATE ${CMAKE_SOURCE_DIR}/src/lib ${CMAKE_SOURCE_DIR}/test)
target_link_libraries(ffmpegTest gtest_main gmock_main libFFmpeg++)

include(GoogleTest)
gtest_discover_tests(ffmpegTest)
