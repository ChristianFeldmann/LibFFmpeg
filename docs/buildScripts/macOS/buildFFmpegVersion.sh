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

if [[ $2 = 56 ]]; then
    echo "Applying build path for FFmpeg 2"
    git apply ../ffmpeg2.patch
fi

echo "Configuring FFmpeg..."
./configure --disable-static --enable-shared --disable-debug --disable-everything --enable-decoder=h264 --enable-demuxer=mov --enable-demuxer=matroska --enable-protocol=file
make clean
make -j 8
git reset --hard

cd ..
mkdir tmpFiles
cd tmpFiles

cp ../FFmpeg/libavcodec/libavcodec.$2.dylib .
cp ../FFmpeg/libavformat/libavformat.$3.dylib .
cp ../FFmpeg/libavutil/libavutil.$4.dylib .
cp ../FFmpeg/libswresample/libswresample.$5.dylib .
cp ../FFmpeg/ffmpeg .
cp ../FFmpeg/ffprobe .

zip ffmpeg-$1.zip *
cd ..
mv tmpFiles/ffmpeg-$1.zip .

rm -rf tmpFiles

