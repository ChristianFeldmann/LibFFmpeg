/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVCodecWrapper.h"

#include <common/InternalTypes.h>

#include <stdexcept>

#include "AVCodecWrapperInternal.h"
#include "CastCodecClasses.h"

namespace ffmpeg::avcodec
{

using ffmpeg::internal::AVCodec;
using ffmpeg::internal::AVCodecID;
using ffmpeg::internal::AVMediaType;
using ffmpeg::internal::AVPixelFormat;
using ffmpeg::internal::AVRational;
using ffmpeg::internal::AVSampleFormat;
using ffmpeg::internal::avcodec::AVCodec_56;
using ffmpeg::internal::avcodec::AVCodec_57;
using ffmpeg::internal::avcodec::AVCodec_58;
using ffmpeg::internal::avcodec::AVCodec_59;
using ffmpeg::internal::avcodec::AVCodec_60;

namespace
{

template <typename T> std::vector<T> convertRawListToVec(const T *rawValues, T terminationValue)
{
  std::vector<T> values;
  int            i   = 0;
  auto           val = rawValues[i++];
  while (val != terminationValue)
  {
    values.push_back(val);
    val = rawValues[i++];
  }
  return values;
}

} // namespace

AVCodecWrapper::AVCodecWrapper(AVCodec *codec, std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
    : codec(codec), ffmpegLibraries(ffmpegLibraries)
{
}

std::string AVCodecWrapper::getName() const
{
  const char *name;
  CAST_AVCODEC_GET_MEMBER(AVCodec, this->codec, name, name);
  return std::string(name);
}

std::string AVCodecWrapper::getLongName() const
{
  const char *longName;
  CAST_AVCODEC_GET_MEMBER(AVCodec, this->codec, longName, long_name);
  return std::string(longName);
}

MediaType AVCodecWrapper::getMediaType() const
{
  AVMediaType mediaType;
  CAST_AVCODEC_GET_MEMBER(AVCodec, this->codec, mediaType, type);
  return ffmpeg::internal::toMediaType(mediaType);
}

AVCodecID AVCodecWrapper::getCodecID() const
{
  AVCodecID codecID;
  CAST_AVCODEC_GET_MEMBER(AVCodec, this->codec, codecID, id);
  return codecID;
}

int AVCodecWrapper::getCapabilities() const
{
  int capabilities;
  CAST_AVCODEC_GET_MEMBER(AVCodec, this->codec, capabilities, capabilities);
  return capabilities;
}

std::vector<Rational> AVCodecWrapper::getSupportedFramerates() const
{
  const AVRational *rates{};
  CAST_AVCODEC_GET_MEMBER(AVCodec, this->codec, rates, supported_framerates);

  std::vector<Rational> framerates;
  int                   i   = 0;
  auto                  val = rates[i++];
  while (val.num != 0 && val.den != 0)
  {
    framerates.push_back({val.num, val.den});
    val = rates[i++];
  }

  return framerates;
}

std::vector<avutil::AVPixFmtDescriptorWrapper> AVCodecWrapper::getPixelFormats() const
{
  const AVPixelFormat *formatsPointer;
  CAST_AVCODEC_GET_MEMBER(AVCodec, this->codec, formatsPointer, pix_fmts);

  std::vector<avutil::AVPixFmtDescriptorWrapper> formats;

  int  i   = 0;
  auto val = formatsPointer[i++];
  while (val != AVPixelFormat::AV_PIX_FMT_NONE)
  {
    formats.push_back(avutil::AVPixFmtDescriptorWrapper(val, this->ffmpegLibraries));
    val = formatsPointer[i++];
  }

  return formats;
}

std::vector<int> AVCodecWrapper::getSupportedSamplerates() const
{
  const int *rates;
  CAST_AVCODEC_GET_MEMBER(AVCodec, this->codec, rates, supported_samplerates);
  return convertRawListToVec(rates, 0);
}

std::vector<AVSampleFormat> AVCodecWrapper::getSampleFormats() const
{
  const AVSampleFormat *formats;
  CAST_AVCODEC_GET_MEMBER(AVCodec, this->codec, formats, sample_fmts);
  return convertRawListToVec(formats, AVSampleFormat(-1));
}

std::vector<uint64_t> AVCodecWrapper::getChannelLayouts() const
{
  const uint64_t *layouts;
  CAST_AVCODEC_GET_MEMBER(AVCodec, this->codec, layouts, channel_layouts);
  return convertRawListToVec(layouts, uint64_t(0));
}

uint8_t AVCodecWrapper::getMaxLowres() const
{
  uint8_t maxLowres;
  CAST_AVCODEC_GET_MEMBER(AVCodec, this->codec, maxLowres, max_lowres);
  return maxLowres;
}

} // namespace ffmpeg::avcodec
