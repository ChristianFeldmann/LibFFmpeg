# How the minimal FFmpeg versions were built

Get the sources from:

> git clone https://git.ffmpeg.org/ffmpeg.git

## Linux (Ubuntu 22.04)

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

## Other hints

For debugging add the following to the configuration parameters:

> --enable-debug --disable-optimizations --disable-mmx --disable-stripping
