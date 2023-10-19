FILE(GLOB_RECURSE TEST_UNIT_SOURCE_FILES *.cpp)
FILE(GLOB_RECURSE TEST_UNIT_HEADER_FILES *.h)

add_executable(unitTest ${TEST_UNIT_SOURCE_FILES} ${TEST_UNIT_HEADER_FILES})

target_include_directories(unitTest PRIVATE ${CMAKE_SOURCE_DIR}/src/lib)
target_link_libraries(unitTest GTest::gtest_main libFFmpeg++)

include(GoogleTest)
gtest_discover_tests(unitTest)
