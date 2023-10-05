/* LibFFmpeg++
 * Copyright (C) 2023 Christian Feldmann [christian.feldmann@gmx.de]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
