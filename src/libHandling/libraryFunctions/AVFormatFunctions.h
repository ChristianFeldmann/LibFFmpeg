/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/Expected.h>
#include <common/FFMpegLibrariesTypes.h>
#include <libHandling/SharedLibraryLoader.h>

namespace LibFFmpeg::functions
{

struct AvFormatFunctions
{
  std::function<void()> av_register_all;
  std::function<int(
      AVFormatContext **ps, const char *url, AVInputFormat *fmt, AVDictionary **options)>
                                                                  avformat_open_input;
  std::function<void(AVFormatContext **s)>                        avformat_close_input;
  std::function<int(AVFormatContext *ic, AVDictionary **options)> avformat_find_stream_info;
  std::function<int(AVFormatContext *s, AVPacket *pkt)>           av_read_frame;
  std::function<int(AVFormatContext *s, int stream_index, int64_t timestamp, int flags)>
                            av_seek_frame;
  std::function<unsigned()> avformat_version;
};

std::optional<AvFormatFunctions> tryBindAVFormatFunctionsFromLibrary(SharedLibraryLoader &lib,
                                                                     Log &                log);

} // namespace LibFFmpeg::functions
