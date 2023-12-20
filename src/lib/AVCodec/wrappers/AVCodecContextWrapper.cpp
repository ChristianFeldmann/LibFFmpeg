/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVCodecContextWrapper.h"

#include <common/Error.h>
#include <common/Functions.h>
#include <common/InternalTypes.h>

#include "AVCodecContextWrapperInternal.h"
#include "CastCodecClasses.h"

namespace ffmpeg::avcodec
{

using ffmpeg::internal::AVCodecContext;
using ffmpeg::internal::AVCodecID;
using ffmpeg::internal::AVColorSpace;
using ffmpeg::internal::AVDictionary;
using ffmpeg::internal::AVMediaType;
using ffmpeg::internal::AVPixelFormat;
using ffmpeg::internal::AVRational;
using ffmpeg::internal::avcodec::AVCodecContext_56;
using ffmpeg::internal::avcodec::AVCodecContext_57;
using ffmpeg::internal::avcodec::AVCodecContext_58;
using ffmpeg::internal::avcodec::AVCodecContext_59;
using ffmpeg::internal::avcodec::AVCodecContext_60;

namespace internal
{

} // namespace internal

AVCodecContextWrapper::AVCodecContextWrapper(AVCodecContext *                  codecContext,
                                             std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
    : codecContext(codecContext), ffmpegLibraries(ffmpegLibraries)
{
  if (codecContext == nullptr)
    throw std::runtime_error("Provided codecContext pointer must not be null");
  if (!ffmpegLibraries)
    throw std::runtime_error("Provided ffmpeg libraries pointer must not be null");
}

std::optional<AVCodecContextWrapper> AVCodecContextWrapper::openContextForDecoding(
    const avformat::AVCodecParametersWrapper &codecParameters,
    std::shared_ptr<IFFmpegLibraries>         ffmpegLibraries)
{
  const auto decoderCodec =
      ffmpegLibraries->avcodec.avcodec_find_decoder(codecParameters.getCodecID());
  if (decoderCodec == nullptr)
    return {};

  auto codecContext = ffmpegLibraries->avcodec.avcodec_alloc_context3(decoderCodec);
  if (codecContext == nullptr)
    return {};

  auto ret = ffmpegLibraries->avcodec.avcodec_parameters_to_context(
      codecContext, codecParameters.getCodecParameters());
  if (ret < 0)
    return {};

  AVDictionary *dictionary = nullptr;
  ret = ffmpegLibraries->avcodec.avcodec_open2(codecContext, decoderCodec, &dictionary);
  if (ret < 0)
    return {};

  return AVCodecContextWrapper(codecContext, ffmpegLibraries);
}

ReturnCode AVCodecContextWrapper::sendPacket(const avcodec::AVPacketWrapper &packet)
{
  const auto avReturnCode =
      this->ffmpegLibraries->avcodec.avcodec_send_packet(this->codecContext, packet.getPacket());
  return toReturnCode(avReturnCode);
}

ReturnCode AVCodecContextWrapper::sendFlushPacket()
{
  const auto avReturnCode =
      this->ffmpegLibraries->avcodec.avcodec_send_packet(this->codecContext, nullptr);
  return toReturnCode(avReturnCode);
}

AVCodecContextWrapper::RevieveFrameResult AVCodecContextWrapper::revieveFrame()
{
  RevieveFrameResult result;
  result.frame.emplace(this->ffmpegLibraries);

  const auto avReturnCode = this->ffmpegLibraries->avcodec.avcodec_receive_frame(
      this->codecContext, result.frame->getFrame());
  result.returnCode = toReturnCode(avReturnCode);

  return result;
}

avutil::MediaType AVCodecContextWrapper::getCodecType() const
{
  AVMediaType type;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, type, codec_type);
  return avutil::toMediaType(type);
}

AVCodecID AVCodecContextWrapper::getCodecID() const
{
  AVCodecID id;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, id, codec_id);
  return id;
}

avutil::PixelFormatDescriptor AVCodecContextWrapper::getPixelFormat() const
{
  AVPixelFormat avPixelFormat;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, avPixelFormat, pix_fmt);
  return avutil::convertAVPixFmtDescriptor(avPixelFormat, this->ffmpegLibraries);
}

Size AVCodecContextWrapper::getSize() const
{
  int width;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, width, width);

  int height;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, height, height);

  return {width, height};
}

avutil::ColorSpace AVCodecContextWrapper::getColorspace() const
{
  AVColorSpace avColorspace;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, avColorspace, colorspace);
  return avutil::toColorspace(avColorspace);
}

Rational AVCodecContextWrapper::getTimeBase() const
{
  AVRational timebase;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, timebase, time_base);
  return Rational({timebase.num, timebase.den});
}

ByteVector AVCodecContextWrapper::getExtradata() const
{
  uint8_t *extradata;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, extradata, extradata);

  int extradataSize;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, extradataSize, extradata_size);

  return copyDataFromRawArray(extradata, extradataSize);
}

} // namespace ffmpeg::avcodec
