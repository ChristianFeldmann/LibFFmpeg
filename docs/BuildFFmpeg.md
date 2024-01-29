# How the minimal FFmpeg versions were built

This folder contains the scripts I used to build the different ffmpeg versions for testing. We are building a very light FFmpeg version that can only decode a few codecs and parse a few containers. We do this to keep the ffmpeg libraries small. For testing of the basic features, we only need a few features. 

We are buuilding from the ffmpeg git repo:

> git clone https://git.ffmpeg.org/ffmpeg.git

## Linux (Ubuntu 22.04)

On linux we can compile ffmpeg natively (you will need the build tools to be installed).

```
git checkout n6.0
./configure --disable-static --enable-shared --disable-debug --disable-everything --enable-decoder=h264 --enable-demuxer=mov --enable-demuxer=matroska --enable-protocol=file
make -j 32

cd ../ffmpeg6.0
cp ../ffmpeg/libavcodec/libavcodec.so.60 .
cp ../ffmpeg/libavformat/libavformat.so.60 .
cp ../ffmpeg/libavutil/libavutil.so.58 .
cp ../ffmpeg/libswresample/libswresample.so.4 .
cp ../ffmpeg/ffmpeg .
cp ../ffmpeg/ffprobe .

zip ffmpeg6.0.zip *
```

## Windows

For windows I used cross compilation from linux (on an Ubuntu 22.04). For this, I installed gcc-mingw-w64-i686 and mingw-w64-i686-dev.

> sudo apt install gcc-mingw-w64-i686 mingw-w64-i686-dev

Then ffmpeg can be configured and built using configure with the correct mongw prefix:

```
./configure --disable-static --enable-shared --disable-debug --disable-everything --enable-decoder=h264 --enable-demuxer=mov --enable-demuxer=matroska --enable-protocol=file --arch=x86_64 --target-os=mingw32 --cross-prefix=i686-w64-mingw32-
make -j 32
```

```
cd ../ffmpeg6.0
cp ../ffmpeg/libavcodec/avcodec-60.dll .
cp ../ffmpeg/libavformat/avformat-60.dll .
cp ../ffmpeg/libavutil/avutil-58.dll .
cp ../ffmpeg/libswresample/swresample-4.dll .
cp ../ffmpeg/ffmpeg.exe .
cp ../ffmpeg/ffprobe.exe .
```

Please see the windows folder for the build scripts.

## Mac

Todo

## Other hints

For debugging add the following to the configuration parameters:

> --enable-debug --disable-optimizations --disable-mmx --disable-stripping
