cmake_minimum_required(VERSION 3.14)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project(LibFFmpeg++)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(FATAL_ERROR "Building for 32 bit targets is not supported.")
endif()

option(ENABLE_TEST "Enable build of Tests" OFF)

add_subdirectory(src/lib)
add_subdirectory(src/app)

if(ENABLE_TEST)
    message(STATUS "Enable building of unit tests")
    add_subdirectory(test)
else()
    message(STATUS "Not building unit tests")
endif()
