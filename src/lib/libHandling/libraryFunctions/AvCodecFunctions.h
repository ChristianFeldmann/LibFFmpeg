/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

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
