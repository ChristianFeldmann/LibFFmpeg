/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVCodecWrapper.h"

#include <common/InternalTypes.h>

#include <stdexcept>

#include "CastCodecClasses.h"

namespace ffmpeg::avcodec
{

namespace internal
{

struct AVCodec_56
{
  const char                            *name;
  const char                            *long_name;
  enum AVMediaType                       type;
  enum AVCodecID                         id;
  int                                    capabilities;
  const AVRational                      *supported_framerates;
  const ffmpeg::internal::AVPixelFormat *pix_fmts;
  const int                             *supported_samplerates;
  const enum AVSampleFormat             *sample_fmts;
  const uint64_t                        *channel_layouts;
  uint8_t                                max_lowres;
  const AVClass                         *priv_class;

  // Actually, there is more here, but nothing more of the public API
};

typedef AVCodec_56 AVCodec_57;
typedef AVCodec_56 AVCodec_58;

struct AVCodec_59
{
  const char                            *name;
  const char                            *long_name;
  enum AVMediaType                       type;
  enum AVCodecID                         id;
  int                                    capabilities;
  uint8_t                                max_lowres;
  const AVRational                      *supported_framerates;
  const ffmpeg::internal::AVPixelFormat *pix_fmts;
  const int                             *supported_samplerates;
  const enum AVSampleFormat             *sample_fmts;
  const uint64_t                        *channel_layouts;
  const AVClass                         *priv_class;

  // Actually, there is more here, but nothing more of the public API
};

typedef AVCodec_59 AVCodec_60;

} // namespace internal

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

AVCodecWrapper::AVCodecWrapper(AVCodec                                  *codec,
                               std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
    : codec(codec), librariesInterface(librariesInterface)
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

AVMediaType AVCodecWrapper::getMediaType() const
{
  AVMediaType mediaType;
  CAST_AVCODEC_GET_MEMBER(AVCodec, this->codec, mediaType, type);
  return mediaType;
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

std::vector<AVRational> AVCodecWrapper::getSupportedFramerates() const
{
  const AVRational *rates;
  CAST_AVCODEC_GET_MEMBER(AVCodec, this->codec, rates, supported_framerates);
  return convertRawListToVec(rates, AVRational({0, 0}));
}

std::vector<avutil::AVPixFmtDescriptorWrapper> AVCodecWrapper::getPixelFormats() const
{
  const ffmpeg::internal::AVPixelFormat *formatsPointer;
  CAST_AVCODEC_GET_MEMBER(AVCodec, this->codec, formatsPointer, pix_fmts);

  std::vector<avutil::AVPixFmtDescriptorWrapper> formats;

  int  i   = 0;
  auto val = formatsPointer[i++];
  while (val != ffmpeg::internal::AVPixelFormat::AV_PIX_FMT_NONE)
  {
    formats.push_back(avutil::AVPixFmtDescriptorWrapper(val, this->librariesInterface));
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
