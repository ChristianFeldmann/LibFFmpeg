/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/InternalTypes.h>
#include <common/Logging.h>
#include <common/Types.h>
#include <libHandling/SharedLibraryLoader.h>

namespace libffmpeg::internal::functions
{

using ReadPacketFunction  = int(void *opaque, uint8_t *buf, int buf_size);
using WritePacketFunction = int(void *opaque, const uint8_t *buf, int buf_size);
using SeekFunction        = int64_t(void *opaque, int64_t offset, int whence);

struct AvFormatFunctions
{
  std::function<unsigned()> avformat_version;
  std::function<void()>     av_register_all;
  std::function<int(
      AVFormatContext **ps, const char *url, AVInputFormat *fmt, AVDictionary **options)>
                                                                  avformat_open_input;
  std::function<void(AVFormatContext **s)>                        avformat_close_input;
  std::function<AVFormatContext *(void)>                          avformat_alloc_context;
  std::function<int(AVFormatContext *ic, AVDictionary **options)> avformat_find_stream_info;
  std::function<int(AVFormatContext *s, AVPacket *pkt)>           av_read_frame;
  std::function<int(AVFormatContext *s, int stream_index, int64_t timestamp, int flags)>
      av_seek_frame;

  std::function<AVIOContext *(unsigned char       *buffer,
                              int                  buffer_size,
                              int                  write_flag,
                              void                *opaque,
                              ReadPacketFunction  *read_packet,
                              WritePacketFunction *write_packet,
                              SeekFunction        *seek)>
                                        avio_alloc_context;
  std::function<void(AVIOContext **ps)> avio_context_free;
};

std::optional<AvFormatFunctions> tryBindAVFormatFunctionsFromLibrary(const SharedLibraryLoader &lib,
                                                                     const LoggingFunction &log);

} // namespace libffmpeg::internal::functions
