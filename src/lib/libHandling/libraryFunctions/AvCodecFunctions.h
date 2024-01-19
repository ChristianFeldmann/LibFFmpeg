/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/InternalTypes.h>
#include <common/Types.h>
#include <common/Version.h>
#include <libHandling/SharedLibraryLoader.h>

namespace ffmpeg::internal::functions
{

struct AvCodecFunctions
{
  std::function<AVCodec *(AVCodecID)>                                    avcodec_find_decoder;
  std::function<AVCodecContext *(const AVCodec *)>                       avcodec_alloc_context3;
  std::function<int(AVCodecContext *, const AVCodec *, AVDictionary **)> avcodec_open2;
  std::function<void(AVCodecContext **)>                                 avcodec_free_context;
  std::function<int(AVPacket *, int)>                                    av_new_packet;
  std::function<int(AVPacket *, const AVPacket *)>                       av_copy_packet;
  std::function<void(AVCodecContext *)>                                  avcodec_flush_buffers;
  std::function<unsigned()>                                              avcodec_version;
  std::function<const char *(AVCodecID)>                                 avcodec_get_name;
  std::function<const AVCodecDescriptor *(AVCodecID)>                    avcodec_descriptor_get;

  // FFmpeg Version 2.x (avcodec 56)
  std::function<void(AVPacket *pkt)>                                       av_free_packet;
  std::function<int(AVCodecContext *, AVFrame *, int *, const AVPacket *)> avcodec_decode_video2;
  std::function<void(AVPacket *pkt)>                                       av_init_packet;

  // FFmpeg >= Version 3.x (>= avcodec 57)
  std::function<AVCodecParameters *()>                            avcodec_parameters_alloc;
  std::function<AVPacket *()>                                     av_packet_alloc;
  std::function<void(AVPacket **)>                                av_packet_free;
  bool                                                            newParametersAPIAvailable{};
  std::function<int(AVCodecContext *, const AVPacket *)>          avcodec_send_packet;
  std::function<int(AVCodecContext *, AVFrame *)>                 avcodec_receive_frame;
  std::function<int(AVCodecContext *, const AVCodecParameters *)> avcodec_parameters_to_context;
};

std::optional<AvCodecFunctions> tryBindAVCodecFunctionsFromLibrary(const SharedLibraryLoader &lib,
                                                                   const Version avCodecVersion,
                                                                   Log          &log);

} // namespace ffmpeg::internal::functions
