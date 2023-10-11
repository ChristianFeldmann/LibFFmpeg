/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVCodecParametersWrapper.h"

#include <wrappers/Functions.h>

#include "CastFormatClasses.h"

#include <cstring>

namespace ffmpeg::avformat
{

namespace
{

/* This is a dummy class that is just here so that we can use the generic
 * CAST_AVFORMAT_GET_MEMBER and CAST_AVFORMAT_SET_MEMBER macros. In reality,
 * we will never do the cast to this struct.
 */
struct AVCodecParameters_56
{
  AVMediaType  codec_type;
  AVCodecID    codec_id;
  uint8_t     *extradata;
  int          extradata_size;
  int          format;
  int          profile;
  int          level;
  int          width;
  int          height;
  AVRational   sample_aspect_ratio;
  AVColorSpace color_space;
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
  AVColorSpace                  color_space;
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

AVMediaType AVCodecParametersWrapper::getCodecType() const
{
  const auto versions = this->librariesInterface->getLibrariesVersion();
  if (versions.avformat.major == 56)
    return AVMEDIA_TYPE_UNKNOWN;

  AVMediaType mediaType;
  CAST_AVFORMAT_GET_MEMBER(
      versions, AVCodecParameters, this->codecParameters, mediaType, codec_type);
  return mediaType;
}

AVCodecID AVCodecParametersWrapper::getCodecID() const
{
  const auto versions = this->librariesInterface->getLibrariesVersion();
  if (versions.avformat.major == 56)
    return AV_CODEC_ID_NONE;

  AVCodecID codecID;
  CAST_AVFORMAT_GET_MEMBER(this->librariesInterface->getLibrariesVersion(),
                           AVCodecParameters,
                           this->codecParameters,
                           codecID,
                           codec_id);
  return codecID;
}

ByteVector AVCodecParametersWrapper::getExtradata() const
{
  const auto versions = this->librariesInterface->getLibrariesVersion();
  if (versions.avformat.major == 56)
    return {};

  uint8_t *extradata;
  CAST_AVFORMAT_GET_MEMBER(
      versions, AVCodecParameters, this->codecParameters, extradata, extradata);

  int extradataSize;
  CAST_AVFORMAT_GET_MEMBER(
      versions, AVCodecParameters, this->codecParameters, extradataSize, extradata_size);

  return copyDataFromRawArray(extradata, extradataSize);
}

Size AVCodecParametersWrapper::getSize() const
{
  const auto versions = this->librariesInterface->getLibrariesVersion();
  if (versions.avformat.major == 56)
    return {};

  int width, height;
  CAST_AVFORMAT_GET_MEMBER(versions, AVCodecParameters, this->codecParameters, width, width);
  CAST_AVFORMAT_GET_MEMBER(versions, AVCodecParameters, this->codecParameters, height, height);

  return {width, height};
}

AVColorSpace AVCodecParametersWrapper::getColorspace() const
{
  const auto versions = this->librariesInterface->getLibrariesVersion();
  if (versions.avformat.major == 56)
    return AVCOL_SPC_UNSPECIFIED;

  AVColorSpace colorspace;
  CAST_AVFORMAT_GET_MEMBER(
      versions, AVCodecParameters, this->codecParameters, colorspace, color_space);
  return colorspace;
}

AVPixelFormat AVCodecParametersWrapper::getPixelFormat() const
{
  const auto versions = this->librariesInterface->getLibrariesVersion();
  if (versions.avformat.major == 56)
    return AV_PIX_FMT_NONE;

  int pixelFormat;
  CAST_AVFORMAT_GET_MEMBER(versions, AVCodecParameters, this->codecParameters, pixelFormat, format);
  return static_cast<AVPixelFormat>(pixelFormat);
}

Ratio AVCodecParametersWrapper::getSampleAspectRatio() const
{
  const auto versions = this->librariesInterface->getLibrariesVersion();
  if (versions.avformat.major == 56)
    return {};

  AVRational sampleAspectRatio;
  CAST_AVFORMAT_GET_MEMBER(
      versions, AVCodecParameters, this->codecParameters, sampleAspectRatio, sample_aspect_ratio);
  return {sampleAspectRatio.num, sampleAspectRatio.den};
}

void AVCodecParametersWrapper::setClearValues()
{
  const auto version = this->librariesInterface->getLibrariesVersion().avformat.major;

  if (version == 57 || version == 58 || version == 59 || version == 60)
  {
    auto p                   = reinterpret_cast<AVCodecParameters_57 *>(this->codecParameters);
    p->codec_type            = AVMEDIA_TYPE_UNKNOWN;
    p->codec_id              = AV_CODEC_ID_NONE;
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
    p->field_order     = AV_FIELD_UNKNOWN;
    p->color_range     = AVCOL_RANGE_UNSPECIFIED;
    p->color_primaries = AVCOL_PRI_UNSPECIFIED;
    p->color_trc       = AVCOL_TRC_UNSPECIFIED;
    p->color_space     = AVCOL_SPC_UNSPECIFIED;
    p->chroma_location = AVCHROMA_LOC_UNSPECIFIED;
    p->video_delay     = 0;
  }
  else
    throw std::runtime_error("Invalid library version");
}

void AVCodecParametersWrapper::setAVMediaType(AVMediaType type)
{
  CAST_AVFORMAT_SET_MEMBER(AVCodecParameters, this->codecParameters, codec_type, type);
}

void AVCodecParametersWrapper::setAVCodecID(AVCodecID id)
{
  CAST_AVFORMAT_SET_MEMBER(AVCodecParameters, this->codecParameters, codec_id, id);
}

void AVCodecParametersWrapper::setExtradata(const ByteVector &data)
{
  uint8_t   *extradata{};
  const auto versions = this->librariesInterface->getLibrariesVersion();
  CAST_AVFORMAT_GET_MEMBER(
      versions, AVCodecParameters, this->codecParameters, extradata, extradata);

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
  CAST_AVFORMAT_SET_MEMBER(AVCodecParameters, this->codecParameters, extradata_size, data.size());
}

void AVCodecParametersWrapper::setSize(Size size)
{
  CAST_AVFORMAT_SET_MEMBER(AVCodecParameters, this->codecParameters, width, size.width);
  CAST_AVFORMAT_SET_MEMBER(AVCodecParameters, this->codecParameters, height, size.height);
}

void AVCodecParametersWrapper::setAVPixelFormat(AVPixelFormat format)
{
  CAST_AVFORMAT_SET_MEMBER(AVCodecParameters, this->codecParameters, format, format);
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
