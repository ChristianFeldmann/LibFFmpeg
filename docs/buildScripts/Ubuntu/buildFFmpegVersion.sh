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

echo "Configuring FFmpeg..."
./configure --disable-static --enable-shared --disable-debug --disable-everything --enable-decoder=h264 --enable-demuxer=mov --enable-demuxer=matroska --enable-protocol=file
make clean
make -j 8
git reset --hard

cd ..
mkdir tmpFiles
cd tmpFiles

cp ../FFmpeg/libavcodec/libavcodec.so.$2 .
cp ../FFmpeg/libavformat/libavformat.so.$3 .
cp ../FFmpeg/libavutil/libavutil.so.$4 .
cp ../FFmpeg/libswresample/libswresample.so.$5 .
cp ../FFmpeg/ffmpeg .
cp ../FFmpeg/ffprobe .

zip ffmpeg-$1.zip *
cd ..
mv tmpFiles/ffmpeg-$1.zip .

rm -rf tmpFiles

