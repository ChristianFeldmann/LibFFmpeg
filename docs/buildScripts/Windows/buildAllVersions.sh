#!/bin/bash

git clone git@github.com:FFmpeg/FFmpeg.git

./buildFFmpegVersion.sh 7.0 61 61 59 5
./buildFFmpegVersion.sh 6.1.1 60 60 58 4
./buildFFmpegVersion.sh 5.1.4 59 59 57 4
./buildFFmpegVersion.sh 4.4.4 58 58 56 3
./buildFFmpegVersion.sh 3.4.13 57 57 55 2
./buildFFmpegVersion.sh 2.8.22 56 56 54 1
