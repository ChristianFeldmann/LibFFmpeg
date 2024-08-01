add_executable(fileProber FileProber.cpp)

target_include_directories(fileProber PRIVATE ${CMAKE_SOURCE_DIR}/src/lib)
target_link_libraries (fileProber libFFmpeg++)

install(TARGETS fileProber RUNTIME DESTINATION bin COMPONENT applications)


add_executable(filePacketComparer FilePacketComparer.cpp)

target_include_directories(filePacketComparer PRIVATE ${CMAKE_SOURCE_DIR}/src/lib)
target_link_libraries (filePacketComparer libFFmpeg++)

install(TARGETS filePacketComparer RUNTIME DESTINATION bin COMPONENT applications)
