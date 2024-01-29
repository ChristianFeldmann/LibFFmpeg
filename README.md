# LibFFmpeg

LibFFmpeg++ is a RAII C++ wrapper that can load the shared FFmpeg libraries of different versions and export them for demuxing of packets as well as for decoding of raw video frames from a file. We use system calls to load the shared ffmpeg libraries (dlopen on linux and mac or LoadLibraryA on Windows). We then check the loaded libraries for all the functions that we need. The library is RAII and takes care of allocating / deleting of internal ffmpeg structures and memory. 

## Use case

This library can be very useful if you write an application in which you want to use ffmpeg for parsing/decoding of files but you don't know which ffmpeg version the user may have installed. 

For example, I use (want to use) this libray in [YUView](https://github.com/IENT/YUView). 
