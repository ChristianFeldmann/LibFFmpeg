#!/bin/bash

if [[ $# -ne 5 ]]; then
    echo "Illegal number of parameters" >&2
    exit 2
fi

if [ ! -d "FFmpeg" ]; then
  echo "FFmpeg directory not found. FFmpeg must be checked out here already." >&2
  exit 2
fi

echo "Building FFmpeg $1"
echo "With library versions:"
echo "  avcodec: $2"
echo "  avformat: $3"
echo "  avutil: $4"
echo "  swresample: $5"

cd FFmpeg
git checkout n$1

if [[ $2 = 57 ]]; then
    echo "Applying build path for FFmpeg 3"
    git apply ../pathForFFmpeg3.patch
fi
if [[ $2 = 56 ]]; then
    echo "Applying build path for FFmpeg 2"
    git apply ../patchForFFmpeg2.patch
fi

echo "Configuring FFmpeg..."
./configure --disable-static --enable-shared --disable-debug --disable-everything --enable-decoder=h264 --enable-demuxer=mov --enable-demuxer=matroska --enable-protocol=file --arch=x86_64 --target-os=mingw32 --cross-prefix=i686-w64-mingw32-
make clean
make -j 8
git reset --hard

cd ..
mkdir tmpFiles
cd tmpFiles

cp ../FFmpeg/libavcodec/avcodec-$2.dll .
cp ../FFmpeg/libavformat/avformat-$3.dll .
cp ../FFmpeg/libavutil/avutil-$4.dll .
cp ../FFmpeg/libswresample/swresample-$5.dll .
cp ../FFmpeg/ffmpeg.exe .
cp ../FFmpeg/ffprobe.exe .

zip ffmpeg-$1.zip *
cd ..
mv tmpFiles/ffmpeg-$1.zip .

rm -rf tmpFiles

