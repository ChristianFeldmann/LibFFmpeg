set(LIB_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/lib)

add_executable(fileProber FileProber.cpp)

target_include_directories(fileProber PRIVATE ${LIB_SOURCE_DIR})
target_link_libraries (fileProber libFFmpeg++)

install(TARGETS fileProber RUNTIME DESTINATION bin COMPONENT applications)
