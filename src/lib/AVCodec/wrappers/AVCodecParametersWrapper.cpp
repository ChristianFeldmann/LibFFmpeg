/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVCodecParametersWrapper.h"

#include <common/Functions.h>
#include <common/InternalTypes.h>

#include "AVCodecParametersWrapperInternal.h"
#include "CastCodecClasses.h"

#include <cstring>

namespace ffmpeg::avcodec
{

namespace
{

using ffmpeg::internal::AVCodecID;
using ffmpeg::internal::AVCodecParameters;
using ffmpeg::internal::AVMediaType;
using ffmpeg::internal::AVRational;
using ffmpeg::internal::avcodec::AVCodecParameters_56;
using ffmpeg::internal::avcodec::AVCodecParameters_57;
using ffmpeg::internal::avcodec::AVCodecParameters_58;
using ffmpeg::internal::avcodec::AVCodecParameters_59;
using ffmpeg::internal::avcodec::AVCodecParameters_60;
using ffmpeg::internal::avcodec::AVCodecParameters_61;

constexpr std::size_t AV_INPUT_BUFFER_PADDING_SIZE = 32;

#define RETURN_IF_VERSION_TOO_OLD(returnValue)                                                     \
  {                                                                                                \
    if (this->ffmpegLibraries->getLibrariesVersion().avcodec.major <= 56)                          \
      return returnValue;                                                                          \
  }

} // namespace

AVCodecParametersWrapper::AVCodecParametersWrapper(
    AVCodecParameters *codecParameters, std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
    : codecParameters(codecParameters), ffmpegLibraries(ffmpegLibraries)
{
  if (codecParameters == nullptr)
    throw std::runtime_error("Provided codec parameters pointer must not be null");
  if (!ffmpegLibraries)
    throw std::runtime_error("Provided ffmpeg libraries pointer must not be null");
}

avutil::MediaType AVCodecParametersWrapper::getCodecType() const
{
  RETURN_IF_VERSION_TOO_OLD(avutil::MediaType::Unknown);

  AVMediaType mediaType;
  CAST_AVCODEC_GET_MEMBER(AVCodecParameters, this->codecParameters, mediaType, codec_type);
  return ffmpeg::avutil::toMediaType(mediaType);
}

AVCodecID AVCodecParametersWrapper::getCodecID() const
{
  RETURN_IF_VERSION_TOO_OLD(ffmpeg::internal::AV_CODEC_ID_NONE);

  AVCodecID codecID;
  CAST_AVCODEC_GET_MEMBER(AVCodecParameters, this->codecParameters, codecID, codec_id);
  return codecID;
}

ByteVector AVCodecParametersWrapper::getExtradata() const
{
  RETURN_IF_VERSION_TOO_OLD({});

  uint8_t *extradata;
  CAST_AVCODEC_GET_MEMBER(AVCodecParameters, this->codecParameters, extradata, extradata);

  int extradataSize;
  CAST_AVCODEC_GET_MEMBER(AVCodecParameters, this->codecParameters, extradataSize, extradata_size);

  return copyDataFromRawArray(extradata, extradataSize);
}

Size AVCodecParametersWrapper::getSize() const
{
  RETURN_IF_VERSION_TOO_OLD({});

  int width, height;
  CAST_AVCODEC_GET_MEMBER(AVCodecParameters, this->codecParameters, width, width);
  CAST_AVCODEC_GET_MEMBER(AVCodecParameters, this->codecParameters, height, height);

  return {width, height};
}

avutil::ColorSpace AVCodecParametersWrapper::getColorspace() const
{
  RETURN_IF_VERSION_TOO_OLD(avutil::ColorSpace::UNSPECIFIED);

  internal::AVColorSpace avColorspace;
  CAST_AVCODEC_GET_MEMBER(AVCodecParameters, this->codecParameters, avColorspace, color_space);

  return avutil::toColorspace(avColorspace);
}

avutil::PixelFormatDescriptor AVCodecParametersWrapper::getPixelFormat() const
{
  RETURN_IF_VERSION_TOO_OLD({});

  int pixelFormatIndex;
  CAST_AVCODEC_GET_MEMBER(AVCodecParameters, this->codecParameters, pixelFormatIndex, format);

  const auto avPixelFormat = static_cast<internal::AVPixelFormat>(pixelFormatIndex);

  return avutil::convertAVPixFmtDescriptor(avPixelFormat, this->ffmpegLibraries);
}

Rational AVCodecParametersWrapper::getSampleAspectRatio() const
{
  RETURN_IF_VERSION_TOO_OLD({});

  AVRational sampleAspectRatio;
  CAST_AVCODEC_GET_MEMBER(
      AVCodecParameters, this->codecParameters, sampleAspectRatio, sample_aspect_ratio);
  return {sampleAspectRatio.num, sampleAspectRatio.den};
}

void AVCodecParametersWrapper::setClearValues()
{
  const auto version = this->ffmpegLibraries->getLibrariesVersion().avformat.major;

  if (version == 57 || version == 58 || version == 59 || version == 60 || version == 61)
  {
    auto p                   = reinterpret_cast<AVCodecParameters_57 *>(this->codecParameters);
    p->codec_type            = ffmpeg::internal::AVMEDIA_TYPE_UNKNOWN;
    p->codec_id              = ffmpeg::internal::AV_CODEC_ID_NONE;
    p->codec_tag             = 0;
    p->extradata             = nullptr;
    p->extradata_size        = 0;
    p->format                = 0;
    p->bit_rate              = 0;
    p->bits_per_coded_sample = 0;
    p->bits_per_raw_sample   = 0;
    p->profile               = 0;
    p->level                 = 0;
    p->width                 = 0;
    p->height                = 0;
    {
      AVRational ratio;
      ratio.num              = 1;
      ratio.den              = 1;
      p->sample_aspect_ratio = ratio;
    }
    p->field_order     = internal::AV_FIELD_UNKNOWN;
    p->color_range     = internal::AVCOL_RANGE_UNSPECIFIED;
    p->color_primaries = internal::AVCOL_PRI_UNSPECIFIED;
    p->color_trc       = internal::AVCOL_TRC_UNSPECIFIED;
    p->color_space     = internal::AVCOL_SPC_UNSPECIFIED;
    p->chroma_location = internal::AVCHROMA_LOC_UNSPECIFIED;
    p->video_delay     = 0;
  }
  else
    throw std::runtime_error("Invalid library version");
}

void AVCodecParametersWrapper::setAVMediaType(avutil::MediaType type)
{
  CAST_AVCODEC_SET_MEMBER(
      AVCodecParameters, this->codecParameters, codec_type, ffmpeg::avutil::toAVMediaType(type));
}

void AVCodecParametersWrapper::setAVCodecID(AVCodecID id)
{
  CAST_AVCODEC_SET_MEMBER(AVCodecParameters, this->codecParameters, codec_id, id);
}

void AVCodecParametersWrapper::setExtradata(const ByteVector &data)
{
  uint8_t   *extradata{};
  const auto versions = this->ffmpegLibraries->getLibrariesVersion();
  CAST_AVCODEC_GET_MEMBER(AVCodecParameters, this->codecParameters, extradata, extradata);

  if (extradata != nullptr)
    this->ffmpegLibraries->avutil.av_freep(&extradata);

  extradata = static_cast<uint8_t *>(
      this->ffmpegLibraries->avutil.av_mallocz(data.size() + AV_INPUT_BUFFER_PADDING_SIZE));

  if (extradata == nullptr)
  {
    CAST_AVCODEC_SET_MEMBER(AVCodecParameters, this->codecParameters, extradata_size, 0);
    throw std::runtime_error("Error allocating memory for extradata");
  }

  std::memcpy(extradata, data.data(), data.size());

  CAST_AVCODEC_SET_MEMBER(AVCodecParameters, this->codecParameters, extradata, extradata);
  CAST_AVCODEC_SET_MEMBER(
      AVCodecParameters, this->codecParameters, extradata_size, static_cast<int>(data.size()));
}

void AVCodecParametersWrapper::setSize(Size size)
{
  CAST_AVCODEC_SET_MEMBER(AVCodecParameters, this->codecParameters, width, size.width);
  CAST_AVCODEC_SET_MEMBER(AVCodecParameters, this->codecParameters, height, size.height);
}

void AVCodecParametersWrapper::setAVPixelFormat(avutil::PixelFormatDescriptor format)
{
  // CAST_AVFORMAT_SET_MEMBER(AVCodecParameters, this->codecParameters, format, format);
}

void AVCodecParametersWrapper::setProfileLevel(int profile, int level)
{
  CAST_AVCODEC_SET_MEMBER(AVCodecParameters, this->codecParameters, profile, profile);
  CAST_AVCODEC_SET_MEMBER(AVCodecParameters, this->codecParameters, level, level);
}

void AVCodecParametersWrapper::setSampleAspectRatio(int num, int den)
{
  AVRational ratio;
  ratio.num = num;
  ratio.den = den;
  CAST_AVCODEC_SET_MEMBER(AVCodecParameters, this->codecParameters, sample_aspect_ratio, ratio);
}

} // namespace ffmpeg::avcodec
