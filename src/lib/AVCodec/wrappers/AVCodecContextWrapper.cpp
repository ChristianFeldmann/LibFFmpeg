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

namespace libffmpeg::avcodec
{

using libffmpeg::internal::AVCodecContext;
using libffmpeg::internal::AVCodecID;
using libffmpeg::internal::AVColorSpace;
using libffmpeg::internal::AVDictionary;
using libffmpeg::internal::AVMediaType;
using libffmpeg::internal::AVPixelFormat;
using libffmpeg::internal::AVRational;
using libffmpeg::internal::avcodec::AVCodecContext_56;
using libffmpeg::internal::avcodec::AVCodecContext_57;
using libffmpeg::internal::avcodec::AVCodecContext_58;
using libffmpeg::internal::avcodec::AVCodecContext_59;
using libffmpeg::internal::avcodec::AVCodecContext_60;
using libffmpeg::internal::avcodec::AVCodecContext_61;
using libffmpeg::internal::avcodec::AVCodecContext_62;

namespace internal
{

} // namespace internal

AVCodecContextWrapper::AVCodecContextWrapper(AVCodecContext                   *codecContext,
                                             std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
    : codecContext(codecContext), ffmpegLibraries(ffmpegLibraries)
{
  if (codecContext == nullptr)
    throw std::runtime_error("Provided codecContext pointer must not be null");
  if (!ffmpegLibraries)
    throw std::runtime_error("Provided ffmpeg libraries pointer must not be null");
}

AVCodecContextWrapper::AVCodecContextWrapper(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
    : ffmpegLibraries(ffmpegLibraries)
{
  if (!ffmpegLibraries)
    throw std::runtime_error("Provided ffmpeg libraries pointer must not be null");
}

AVCodecContextWrapper &
AVCodecContextWrapper::operator=(AVCodecContextWrapper &&codecContextWrapper) noexcept
{
  this->codecContext               = codecContextWrapper.codecContext;
  this->codecContextOwnership      = codecContextWrapper.codecContextOwnership;
  codecContextWrapper.codecContext = nullptr;
  this->ffmpegLibraries            = std::move(codecContextWrapper.ffmpegLibraries);
  return *this;
}

AVCodecContextWrapper::AVCodecContextWrapper(AVCodecContextWrapper &&codecContextWrapper) noexcept
    : codecContext(codecContextWrapper.codecContext),
      codecContextOwnership(codecContextWrapper.codecContextOwnership),
      ffmpegLibraries(std::move(codecContextWrapper.ffmpegLibraries))
{
}

AVCodecContextWrapper::~AVCodecContextWrapper()
{
  if (this->codecContext && this->codecContextOwnership)
    this->ffmpegLibraries->avcodec.avcodec_free_context(&this->codecContext);
}

bool AVCodecContextWrapper::openContextForDecoding(
    const avcodec::AVCodecParametersWrapper &codecParameters)
{
  const auto decoderCodec =
      ffmpegLibraries->avcodec.avcodec_find_decoder(codecParameters.getCodecID());
  if (decoderCodec == nullptr)
    return false;

  this->codecContext = ffmpegLibraries->avcodec.avcodec_alloc_context3(decoderCodec);
  if (this->codecContext == nullptr)
    return false;

  this->codecContextOwnership = true;

  auto ret = ffmpegLibraries->avcodec.avcodec_parameters_to_context(
      this->codecContext, codecParameters.getCodecParameters());
  if (ret < 0)
    return false;

  AVDictionary *dictionary = nullptr;
  ret = ffmpegLibraries->avcodec.avcodec_open2(this->codecContext, decoderCodec, &dictionary);
  if (ret < 0)
    return false;

  return true;
}

bool AVCodecContextWrapper::openContextForDecoding()
{
  if (this->codecContext == nullptr)
    return false;

  const auto decoderCodec = ffmpegLibraries->avcodec.avcodec_find_decoder(this->getCodecID());
  if (decoderCodec == nullptr)
    return false;

  AVDictionary *dictionary = nullptr;
  const auto    ret =
      ffmpegLibraries->avcodec.avcodec_open2(this->codecContext, decoderCodec, &dictionary);
  if (ret < 0)
    return false;

  return true;
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

AVCodecContextWrapper::DecodeResult AVCodecContextWrapper::revieveFrame()
{
  DecodeResult result;

  result.frame.emplace(this->ffmpegLibraries);
  const auto avReturnCode = this->ffmpegLibraries->avcodec.avcodec_receive_frame(
      this->codecContext, result.frame->getFrame());
  result.returnCode = toReturnCode(avReturnCode);

  return result;
}

AVCodecContextWrapper::DecodeResult
AVCodecContextWrapper::decodeVideo2(const avcodec::AVPacketWrapper &packet)
{
  DecodeResult result;

  if (this->ffmpegLibraries->getLibrariesVersion().avcodec.major != 56)
  {
    result.returnCode = ReturnCode::Unknown;
    return result;
  }

  result.frame.emplace(this->ffmpegLibraries);

  int        frameRecieved = 0;
  const auto avReturnCode  = this->ffmpegLibraries->avcodec.avcodec_decode_video2(
      this->codecContext, result.frame->getFrame(), &frameRecieved, packet.getPacket());

  if (frameRecieved == 0)
    result.frame.reset();

  result.returnCode = toReturnCode(avReturnCode);
  return result;
}

avutil::MediaType AVCodecContextWrapper::getCodecType() const
{
  AVMediaType type{};
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, type, codec_type);
  return avutil::toMediaType(type);
}

AVCodecID AVCodecContextWrapper::getCodecID() const
{
  AVCodecID id{};
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, id, codec_id);
  return id;
}

avutil::PixelFormatDescriptor AVCodecContextWrapper::getPixelFormat() const
{
  AVPixelFormat avPixelFormat{};
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, avPixelFormat, pix_fmt);
  return avutil::convertAVPixFmtDescriptor(avPixelFormat, this->ffmpegLibraries);
}

Size AVCodecContextWrapper::getSize() const
{
  int width{};
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, width, width);

  int height{};
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, height, height);

  return {.width = width, .height = height};
}

avutil::ColorSpace AVCodecContextWrapper::getColorspace() const
{
  AVColorSpace avColorspace{};
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, avColorspace, colorspace);
  return avutil::toColorspace(avColorspace);
}

Rational AVCodecContextWrapper::getTimeBase() const
{
  AVRational timebase{};
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, timebase, time_base);
  return fromAVRational(timebase);
}

ByteVector AVCodecContextWrapper::getExtradata() const
{
  uint8_t *extradata{};
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, extradata, extradata);

  int extradataSize{};
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, extradataSize, extradata_size);

  return copyDataFromRawArray(extradata, extradataSize);
}

} // namespace libffmpeg::avcodec
