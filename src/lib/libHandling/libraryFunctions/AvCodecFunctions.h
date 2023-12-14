/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/InternalTypes.h>
#include <common/Types.h>
#include <libHandling/SharedLibraryLoader.h>

namespace ffmpeg::internal::functions
{

struct AvCodecFunctions
{
  std::function<AVCodec *(AVCodecID)>                                    avcodec_find_decoder;
  std::function<AVCodecContext *(const AVCodec *)>                       avcodec_alloc_context3;
  std::function<int(AVCodecContext *, const AVCodec *, AVDictionary **)> avcodec_open2;
  std::function<void(AVCodecContext **)>                                 avcodec_free_context;
  std::function<AVPacket *()>                                            av_packet_alloc;
  std::function<void(AVPacket **)>                                       av_packet_free;
  std::function<int(AVPacket *, int)>                                    av_new_packet;
  std::function<void(AVPacket *)>                                        av_packet_unref;
  std::function<void(AVCodecContext *)>                                  avcodec_flush_buffers;
  std::function<unsigned()>                                              avcodec_version;
  std::function<const char *(AVCodecID)>                                 avcodec_get_name;
  std::function<AVCodecParameters *()>                                   avcodec_parameters_alloc;
  std::function<const AVCodecDescriptor *(AVCodecID)>                    avcodec_descriptor_get;
  // The following functions are part of the new API.
  // We will check if it is available. If not, we will use the old decoding API.
  bool                                                            newParametersAPIAvailable{};
  std::function<int(AVCodecContext *, const AVPacket *)>          avcodec_send_packet;
  std::function<int(AVCodecContext *, AVFrame *)>                 avcodec_receive_frame;
  std::function<int(AVCodecContext *, const AVCodecParameters *)> avcodec_parameters_to_context;
  // This is the old API
  std::function<void()> avcodec_decode_video2;
};

std::optional<AvCodecFunctions> tryBindAVCodecFunctionsFromLibrary(const SharedLibraryLoader &lib,
                                                                   Log                       &log);

} // namespace ffmpeg::internal::functions
