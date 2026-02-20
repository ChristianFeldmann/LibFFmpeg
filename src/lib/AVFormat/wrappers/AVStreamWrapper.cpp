/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVStreamWrapper.h"

#include <AVCodec/wrappers/AVPacketWrapper.h>
#include <common/Functions.h>

#include "AVStreamWrapperInternal.h"
#include "CastFormatClasses.h"

namespace libffmpeg::avformat
{

namespace
{

using libffmpeg::internal::AVCodecContext;
using libffmpeg::internal::AVCodecID;
using libffmpeg::internal::AVCodecParameters;
using libffmpeg::internal::AVRational;
using libffmpeg::internal::AVStream;
using libffmpeg::internal::AVStreamParseType;
using libffmpeg::internal::avformat::AVStream_56;
using libffmpeg::internal::avformat::AVStream_57;
using libffmpeg::internal::avformat::AVStream_58;
using libffmpeg::internal::avformat::AVStream_59;
using libffmpeg::internal::avformat::AVStream_60;
using libffmpeg::internal::avformat::AVStream_61;
using libffmpeg::internal::avformat::AVStream_62;

} // namespace

AVStreamWrapper::AVStreamWrapper(AVStream                         *stream,
                                 std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
    : stream(stream), ffmpegLibraries(ffmpegLibraries)
{
  if (stream == nullptr)
    throw std::runtime_error("Provided stream pointer must not be null");
  if (!ffmpegLibraries)
    throw std::runtime_error("Provided ffmpeg libraries pointer must not be null");
}

int AVStreamWrapper::getIndex() const
{
  int index{};
  CAST_AVFORMAT_GET_MEMBER(AVStream, this->stream, index, index);
  return index;
}

avutil::MediaType AVStreamWrapper::getCodecType() const
{
  if (const auto codecParameters = this->getCodecParameters())
    return codecParameters->getCodecType();

  if (const auto codecContext = this->getCodecContext())
    return codecContext->getCodecType();

  return avutil::MediaType::Unknown;
}

AVCodecID AVStreamWrapper::getCodecID() const
{
  if (const auto codecParameters = this->getCodecParameters())
    return codecParameters->getCodecID();

  if (const auto codecContext = this->getCodecContext())
    return codecContext->getCodecID();

  return libffmpeg::internal::AV_CODEC_ID_NONE;
}

Rational AVStreamWrapper::getAverageFrameRate() const
{
  AVRational frameRate{};
  CAST_AVFORMAT_GET_MEMBER(AVStream, this->stream, frameRate, avg_frame_rate);
  return fromAVRational(frameRate);
}

Rational AVStreamWrapper::getTimeBase() const
{
  AVRational timebase{};
  CAST_AVFORMAT_GET_MEMBER(AVStream, this->stream, timebase, time_base);

  if (timebase.den != 0 && timebase.num != 0)
    return fromAVRational(timebase);

  // The stream time_base seems not to be set. Try the time_base in the codec.
  if (const auto codecContext = this->getCodecContext())
    return codecContext->getTimeBase();

  return {};
}

Size AVStreamWrapper::getFrameSize() const
{
  if (const auto codecParameters = this->getCodecParameters())
    return codecParameters->getSize();

  if (const auto codecContext = this->getCodecContext())
    return codecContext->getSize();

  return {};
}

avutil::ColorSpace AVStreamWrapper::getColorspace() const
{
  if (const auto codecParameters = this->getCodecParameters())
    return codecParameters->getColorspace();

  if (const auto codecContext = this->getCodecContext())
    return codecContext->getColorspace();

  return avutil::ColorSpace::UNSPECIFIED;
}

avutil::PixelFormatDescriptor AVStreamWrapper::getPixelFormat() const
{
  if (const auto codecParameters = this->getCodecParameters())
    return codecParameters->getPixelFormat();

  if (const auto codecContext = this->getCodecContext())
    return codecContext->getPixelFormat();

  return {};
}

ByteVector AVStreamWrapper::getExtradata() const
{
  if (const auto codecParameters = this->getCodecParameters())
    return codecParameters->getExtradata();

  if (const auto codecContext = this->getCodecContext())
    return codecContext->getExtradata();

  return {};
}

std::optional<avcodec::CodecDescriptor> AVStreamWrapper::getCodecDescriptor() const
{
  const auto codecID = this->getCodecID();

  const auto descriptor = this->ffmpegLibraries->avcodec.avcodec_descriptor_get(codecID);

  if (descriptor)
    return avcodec::convertAVCodecDescriptor(descriptor,
                                             this->ffmpegLibraries->getLibrariesVersion().avcodec);
  return {};
}

std::optional<avcodec::AVCodecContextWrapper> AVStreamWrapper::getCodecContext() const
{
  const auto version = this->ffmpegLibraries->getLibrariesVersion().avformat.major;
  if (version == 56)
  {
    const auto p = reinterpret_cast<AVStream_56 *>(this->stream);
    if (p->codec != nullptr)
      return avcodec::AVCodecContextWrapper(p->codec, this->ffmpegLibraries);
  }
  if (version == 57)
  {
    const auto p = reinterpret_cast<AVStream_57 *>(this->stream);
    if (p->codec != nullptr)
      return avcodec::AVCodecContextWrapper(p->codec, this->ffmpegLibraries);
  }
  if (version == 58)
  {
    const auto p = reinterpret_cast<AVStream_58 *>(this->stream);
    if (p->codec != nullptr)
      return avcodec::AVCodecContextWrapper(p->codec, this->ffmpegLibraries);
  }

  return {};
};

std::optional<avcodec::AVCodecParametersWrapper> AVStreamWrapper::getCodecParameters() const
{
  const auto version = this->ffmpegLibraries->getLibrariesVersion().avformat.major;
  if (version == 56)
  {
    return {};
  }
  if (version == 57)
  {
    const auto p = reinterpret_cast<AVStream_57 *>(this->stream);
    if (p->codecpar != nullptr)
      return avcodec::AVCodecParametersWrapper(p->codecpar, this->ffmpegLibraries);
  }
  if (version == 58)
  {
    const auto p = reinterpret_cast<AVStream_58 *>(this->stream);
    if (p->codecpar != nullptr)
      return avcodec::AVCodecParametersWrapper(p->codecpar, this->ffmpegLibraries);
  }
  if (version == 59)
  {
    const auto p = reinterpret_cast<AVStream_59 *>(this->stream);
    if (p->codecpar != nullptr)
      return avcodec::AVCodecParametersWrapper(p->codecpar, this->ffmpegLibraries);
  }
  if (version == 60)
  {
    const auto p = reinterpret_cast<AVStream_60 *>(this->stream);
    if (p->codecpar != nullptr)
      return avcodec::AVCodecParametersWrapper(p->codecpar, this->ffmpegLibraries);
  }
  if (version == 61)
  {
    const auto p = reinterpret_cast<AVStream_61 *>(this->stream);
    if (p->codecpar != nullptr)
      return avcodec::AVCodecParametersWrapper(p->codecpar, this->ffmpegLibraries);
  }
  if (version == 62)
  {
    const auto p = reinterpret_cast<AVStream_62 *>(this->stream);
    if (p->codecpar != nullptr)
      return avcodec::AVCodecParametersWrapper(p->codecpar, this->ffmpegLibraries);
  }

  return {};
}

} // namespace libffmpeg::avformat
