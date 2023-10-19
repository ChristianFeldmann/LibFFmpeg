FILE(GLOB_RECURSE LIB_SOURCE_FILES *.cpp)
FILE(GLOB_RECURSE LIB_HEADER_FILES *.h)

add_library(libFFmpeg++ STATIC ${LIB_SOURCE_FILES} ${LIB_HEADER_FILES})

target_include_directories(libFFmpeg++ PRIVATE ${CMAKE_SOURCE_DIR}/src/lib)
target_link_libraries(libFFmpeg++ ${CMAKE_DL_LIBS})

set_target_properties(libFFmpeg++ PROPERTIES OUTPUT_NAME libFFmpeg++)
#set_target_properties(libFFmpeg++ PROPERTIES PUBLIC_HEADER "vcaLib.h")

install(TARGETS libFFmpeg++ LIBRARY DESTINATION lib ARCHIVE DESTINATION lib COMPONENT libraries PUBLIC_HEADER DESTINATION include)
