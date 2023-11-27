/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVCodecParametersWrapper.h"

#include <common/InternalTypes.h>
#include <wrappers/Functions.h>

#include "CastFormatClasses.h"

#include <cstring>

namespace ffmpeg::avformat
{

namespace
{

using ffmpeg::internal::AVChromaLocation;
using ffmpeg::internal::AVCodecID;
using ffmpeg::internal::AVCodecParameters;
using ffmpeg::internal::AVColorPrimaries;
using ffmpeg::internal::AVColorRange;
using ffmpeg::internal::AVColorTransferCharacteristic;
using ffmpeg::internal::AVFieldOrder;
using ffmpeg::internal::AVMediaType;
using ffmpeg::internal::AVRational;

constexpr std::size_t AV_INPUT_BUFFER_PADDING_SIZE = 32;

#define RETURN_IF_VERSION_TOO_OLD(returnValue)                                                     \
  {                                                                                                \
    if (this->librariesInterface->getLibrariesVersion().avformat.major <= 56)                      \
      return returnValue;                                                                          \
  }

/* This is a dummy class that is just here so that we can use the generic
 * CAST_AVFORMAT_GET_MEMBER and CAST_AVFORMAT_SET_MEMBER macros. In reality,
 * we will never do the cast to this struct.
 */
struct AVCodecParameters_56
{
  AVMediaType            codec_type;
  AVCodecID              codec_id;
  uint8_t               *extradata;
  int                    extradata_size;
  int                    format;
  int                    profile;
  int                    level;
  int                    width;
  int                    height;
  AVRational             sample_aspect_ratio;
  internal::AVColorSpace color_space;
};

// AVCodecParameters is part of avcodec.
struct AVCodecParameters_57
{
  AVMediaType                   codec_type;
  AVCodecID                     codec_id;
  uint32_t                      codec_tag;
  uint8_t                      *extradata;
  int                           extradata_size;
  int                           format;
  int64_t                       bit_rate;
  int                           bits_per_coded_sample;
  int                           bits_per_raw_sample;
  int                           profile;
  int                           level;
  int                           width;
  int                           height;
  AVRational                    sample_aspect_ratio;
  AVFieldOrder                  field_order;
  AVColorRange                  color_range;
  AVColorPrimaries              color_primaries;
  AVColorTransferCharacteristic color_trc;
  internal::AVColorSpace        color_space;
  AVChromaLocation              chroma_location;
  int                           video_delay;

  // Actually, there is more here, but the variables above are the only we need.
};

typedef AVCodecParameters_57 AVCodecParameters_58;
typedef AVCodecParameters_57 AVCodecParameters_59;
typedef AVCodecParameters_57 AVCodecParameters_60;

} // namespace

AVCodecParametersWrapper::AVCodecParametersWrapper(
    AVCodecParameters                        *codecParameters,
    std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
    : codecParameters(codecParameters), librariesInterface(librariesInterface)
{
}

MediaType AVCodecParametersWrapper::getCodecType() const
{
  RETURN_IF_VERSION_TOO_OLD(MediaType::Unknown);

  AVMediaType mediaType;
  CAST_AVFORMAT_GET_MEMBER(AVCodecParameters, this->codecParameters, mediaType, codec_type);
  return ffmpeg::internal::toMediaType(mediaType);
}

AVCodecID AVCodecParametersWrapper::getCodecID() const
{
  RETURN_IF_VERSION_TOO_OLD(ffmpeg::internal::AV_CODEC_ID_NONE);

  AVCodecID codecID;
  CAST_AVFORMAT_GET_MEMBER(AVCodecParameters, this->codecParameters, codecID, codec_id);
  return codecID;
}

ByteVector AVCodecParametersWrapper::getExtradata() const
{
  RETURN_IF_VERSION_TOO_OLD({});

  uint8_t *extradata;
  CAST_AVFORMAT_GET_MEMBER(AVCodecParameters, this->codecParameters, extradata, extradata);

  int extradataSize;
  CAST_AVFORMAT_GET_MEMBER(AVCodecParameters, this->codecParameters, extradataSize, extradata_size);

  return copyDataFromRawArray(extradata, extradataSize);
}

Size AVCodecParametersWrapper::getSize() const
{
  RETURN_IF_VERSION_TOO_OLD({});

  int width, height;
  CAST_AVFORMAT_GET_MEMBER(AVCodecParameters, this->codecParameters, width, width);
  CAST_AVFORMAT_GET_MEMBER(AVCodecParameters, this->codecParameters, height, height);

  return {width, height};
}

ColorSpace AVCodecParametersWrapper::getColorspace() const
{
  RETURN_IF_VERSION_TOO_OLD(ColorSpace::UNSPECIFIED);

  internal::AVColorSpace avColorspace;
  CAST_AVFORMAT_GET_MEMBER(AVCodecParameters, this->codecParameters, avColorspace, color_space);

  return internal::toColorspace(avColorspace);
}

avutil::AVPixFmtDescriptorWrapper AVCodecParametersWrapper::getPixelFormat() const
{
  RETURN_IF_VERSION_TOO_OLD({});

  int pixelFormatIndex;
  CAST_AVFORMAT_GET_MEMBER(AVCodecParameters, this->codecParameters, pixelFormatIndex, format);

  const auto avPixelFormat = static_cast<internal::AVPixelFormat>(pixelFormatIndex);

  return avutil::AVPixFmtDescriptorWrapper(avPixelFormat, this->librariesInterface);
}

Rational AVCodecParametersWrapper::getSampleAspectRatio() const
{
  RETURN_IF_VERSION_TOO_OLD({});

  AVRational sampleAspectRatio;
  CAST_AVFORMAT_GET_MEMBER(
      AVCodecParameters, this->codecParameters, sampleAspectRatio, sample_aspect_ratio);
  return {sampleAspectRatio.num, sampleAspectRatio.den};
}

void AVCodecParametersWrapper::setClearValues()
{
  const auto version = this->librariesInterface->getLibrariesVersion().avformat.major;

  if (version == 57 || version == 58 || version == 59 || version == 60)
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

void AVCodecParametersWrapper::setAVMediaType(MediaType type)
{
  CAST_AVFORMAT_SET_MEMBER(
      AVCodecParameters, this->codecParameters, codec_type, ffmpeg::internal::toAVMediaType(type));
}

void AVCodecParametersWrapper::setAVCodecID(AVCodecID id)
{
  CAST_AVFORMAT_SET_MEMBER(AVCodecParameters, this->codecParameters, codec_id, id);
}

void AVCodecParametersWrapper::setExtradata(const ByteVector &data)
{
  uint8_t   *extradata{};
  const auto versions = this->librariesInterface->getLibrariesVersion();
  CAST_AVFORMAT_GET_MEMBER(AVCodecParameters, this->codecParameters, extradata, extradata);

  if (extradata != nullptr)
    this->librariesInterface->avutil.av_freep(&extradata);

  extradata = static_cast<uint8_t *>(
      this->librariesInterface->avutil.av_mallocz(data.size() + AV_INPUT_BUFFER_PADDING_SIZE));

  if (extradata == nullptr)
  {
    CAST_AVFORMAT_SET_MEMBER(AVCodecParameters, this->codecParameters, extradata_size, 0);
    throw std::runtime_error("Error allocating memory for extradata");
  }

  std::memcpy(extradata, data.data(), data.size());

  CAST_AVFORMAT_SET_MEMBER(AVCodecParameters, this->codecParameters, extradata, extradata);
  CAST_AVFORMAT_SET_MEMBER(
      AVCodecParameters, this->codecParameters, extradata_size, static_cast<int>(data.size()));
}

void AVCodecParametersWrapper::setSize(Size size)
{
  CAST_AVFORMAT_SET_MEMBER(AVCodecParameters, this->codecParameters, width, size.width);
  CAST_AVFORMAT_SET_MEMBER(AVCodecParameters, this->codecParameters, height, size.height);
}

void AVCodecParametersWrapper::setAVPixelFormat(avutil::AVPixFmtDescriptorWrapper format)
{
  // CAST_AVFORMAT_SET_MEMBER(AVCodecParameters, this->codecParameters, format, format);
}

void AVCodecParametersWrapper::setProfileLevel(int profile, int level)
{
  CAST_AVFORMAT_SET_MEMBER(AVCodecParameters, this->codecParameters, profile, profile);
  CAST_AVFORMAT_SET_MEMBER(AVCodecParameters, this->codecParameters, level, level);
}

void AVCodecParametersWrapper::setSampleAspectRatio(int num, int den)
{
  AVRational ratio;
  ratio.num = num;
  ratio.den = den;
  CAST_AVFORMAT_SET_MEMBER(AVCodecParameters, this->codecParameters, sample_aspect_ratio, ratio);
}

} // namespace ffmpeg::avformat
