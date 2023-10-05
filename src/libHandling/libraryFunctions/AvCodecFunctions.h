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

struct AvCodecFunctions
{
  std::function<AVCodec *(AVCodecID id)>                avcodec_find_decoder;
  std::function<AVCodecContext *(const AVCodec *codec)> avcodec_alloc_context3;
  std::function<int(AVCodecContext *avctx, const AVCodec *codec, AVDictionary **options)>
                                              avcodec_open2;
  std::function<void(AVCodecContext **avctx)> avcodec_free_context;
  std::function<AVPacket *()>                 av_packet_alloc;
  std::function<void(AVPacket **pkt)>         av_packet_free;
  std::function<void(AVPacket *pkt)>          av_init_packet;
  std::function<void(AVPacket *pkt)>          av_packet_unref;
  std::function<void(AVCodecContext *avctx)>  avcodec_flush_buffers;
  std::function<unsigned()>                   avcodec_version;
  std::function<const char *(AVCodecID id)>   avcodec_get_name;
  std::function<AVCodecParameters *()>        avcodec_parameters_alloc;
  // The following functions are part of the new API.
  // We will check if it is available. If not, we will use the old decoding API.
  bool                                                             newParametersAPIAvailable{};
  std::function<int(AVCodecContext *avctx, const AVPacket *avpkt)> avcodec_send_packet;
  std::function<int(AVCodecContext *avctx, AVFrame *frame)>        avcodec_receive_frame;
  std::function<int(AVCodecContext *codec, const AVCodecParameters *par)>
      avcodec_parameters_to_context;
  // This is the old API
  std::function<void()> avcodec_decode_video2;
};

std::optional<AvCodecFunctions> tryBindAVCodecFunctionsFromLibrary(SharedLibraryLoader &lib,
                                                                   Log &                log);

} // namespace LibFFmpeg::functions
