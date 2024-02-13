# LibFFmpeg

LibFFmpeg++ is a RAII C++ wrapper that can load the shared FFmpeg libraries of different versions and export them for demuxing of packets as well as for decoding of raw video frames from a file. We use system calls to load the shared ffmpeg libraries (dlopen on linux and mac or LoadLibraryA on Windows). We then check the loaded libraries for all the functions that we need. The library is RAII and takes care of allocating / deleting of internal ffmpeg structures and memory.

The wrapper is designed to be easy to use and "just work" on windows, linux and macOS. There are tests that check the operation on the supported platforms. 

## Use case

This library can be very useful if you write an application in which you want to use ffmpeg for parsing/decoding of files but you don't know which ffmpeg version the user may have installed. 

For example, I use (want to use) this libray in [YUView](https://github.com/IENT/YUView). 

## How the FFmpeg libraries are loaded

For LibFFmpeg, the libraries 'avutil', 'avcodec', 'avformat' and 'swresample' are needed to open the libraries wrapper. The version of the libraries does not matter. All versions starting with FFmpeg 2 are supported and we try to load the libraries from newest to oldest supported version.

There are 2 ways to load the FFmpeg libraries:
  - Load from a fixed path. In this case we will look for the needed av libraries only in the given path and nowhere else. 
  - If the given path is empty, we leave looking for the libraries to the operating system. In this case it depends on the OS which directories are searched. Some more detail on the paths is given in the [following section](#how-to-install-precompiled-ffmpeg).

## How to install precompiled FFmpeg

There are many ways how to obtain the FFmpeg libraries. I will just list the ones that are tested.

### Windows

On windows, precompiled libraries can be downloaded from the [FFmpeg download page](https://www.ffmpeg.org/download.html) or they can be installed with chocolately:

> choco install ffmpeg-shared

However, neither of these options automatically adds the path to the libraries to a search path for LibFFmpeg. So it is required to either add the path to the libraries to the 'PATH' environment variable or to provide the path manually when opening the libraries using LibFFmpeg.

### Ubuntu

On Ubuntu, ffmpeg can be installed with 

> sudo apt get install ffmpeg

This should put the libraries into a path that is discoverable by LibFFmpeg.

### MacOS

On Mac, homebrew is the tool of choise:

> brew install ffmpeg

There is only one special case with intel and ARM based macs. On Intel, brew adds the libraries to a path that is in the library search paths. On ARM, however, this is not the case. For this reason, we are specifically checking the folder '/opt/homebrew/lib' so that the behavior is identical to the behavior on intel mac.
