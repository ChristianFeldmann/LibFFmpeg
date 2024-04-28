TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++17
CONFIG -= debug_and_release
CONFIG += object_parallel_to_source

SOURCES += $$files(src/lib/*.cpp, true)
HEADERS += $$files(src/lib/*.h, true)

INCLUDEPATH += src/lib
