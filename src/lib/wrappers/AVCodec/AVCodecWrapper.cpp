/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVCodecWrapper.h"
#include <stdexcept>

namespace ffmpeg::avcodec
{

namespace
{

typedef struct AVCodec_56_57_58
{
  const char                *name;
  const char                *long_name;
  enum AVMediaType           type;
  enum AVCodecID             id;
  int                        capabilities;
  const AVRational          *supported_framerates;
  const enum AVPixelFormat  *pix_fmts;
  const int                 *supported_samplerates;
  const enum AVSampleFormat *sample_fmts;
  const uint64_t            *channel_layouts;
  uint8_t                    max_lowres;
  const AVClass             *priv_class;

  // Actually, there is more here, but nothing more of the public API
} AVCodec_56_57_58;

typedef struct AVCodec_59
{
  const char                *name;
  const char                *long_name;
  enum AVMediaType           type;
  enum AVCodecID             id;
  int                        capabilities;
  uint8_t                    max_lowres;
  const AVRational          *supported_framerates;
  const enum AVPixelFormat  *pix_fmts;
  const int                 *supported_samplerates;
  const enum AVSampleFormat *sample_fmts;
  const uint64_t            *channel_layouts;
  const AVClass             *priv_class;

  // Actually, there is more here, but nothing more of the public API
} AVCodec_59;

#define CAST_POINTER_AND_RETURN_MEMBER(member)                                                     \
  {                                                                                                \
    if (this->libraryVersions.avcodec.major == 56 || this->libraryVersions.avcodec.major == 57 ||  \
        this->libraryVersions.avcodec.major == 58)                                                 \
    {                                                                                              \
      const auto p = reinterpret_cast<AVCodec_56_57_58 *>(this->codec);                            \
      return p->member;                                                                            \
    }                                                                                              \
    else if (this->libraryVersions.avcodec.major == 59)                                            \
    {                                                                                              \
      const auto p = reinterpret_cast<AVCodec_59 *>(codec);                                        \
      return p->member;                                                                            \
    }                                                                                              \
    else                                                                                           \
      throw std::runtime_error("Invalid library version");                                         \
  }

#define CAST_POINTER_AND_RETURN_ARRAY_MEMBER(member, terminationValue)                             \
  {                                                                                                \
    if (this->codec)                                                                               \
      throw std::runtime_error("Nullptr exception");                                               \
    if (this->libraryVersions.avcodec.major == 56 || this->libraryVersions.avcodec.major == 57 ||  \
        this->libraryVersions.avcodec.major == 58)                                                 \
    {                                                                                              \
      const auto p = reinterpret_cast<AVCodec_56_57_58 *>(this->codec);                            \
      return convertRawListToVec(p->member, terminationValue);                                     \
    }                                                                                              \
    else if (this->libraryVersions.avcodec.major == 59)                                            \
    {                                                                                              \
      const auto p = reinterpret_cast<AVCodec_59 *>(codec);                                        \
      return convertRawListToVec(p->member, terminationValue);                                     \
    }                                                                                              \
    else                                                                                           \
      throw std::runtime_error("Invalid library version");                                         \
  }

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

AVCodecWrapper::AVCodecWrapper(AVCodec *codec, const LibraryVersions &libraryVersions)
    : codec(codec), libraryVersions(libraryVersions)
{
}

std::string AVCodecWrapper::getName() const
{
  CAST_POINTER_AND_RETURN_MEMBER(name);
}

std::string AVCodecWrapper::getLongName() const
{
  CAST_POINTER_AND_RETURN_MEMBER(long_name);
}

AVMediaType AVCodecWrapper::getMediaType() const
{
  CAST_POINTER_AND_RETURN_MEMBER(type);
}

AVCodecID AVCodecWrapper::getCodecID() const
{
  CAST_POINTER_AND_RETURN_MEMBER(id);
}

int AVCodecWrapper::getCapabilities() const
{
  CAST_POINTER_AND_RETURN_MEMBER(capabilities);
}

std::vector<AVRational> AVCodecWrapper::getSupportedFramerates() const
{
  CAST_POINTER_AND_RETURN_ARRAY_MEMBER(supported_framerates, AVRational({0, 0}));
}

std::vector<AVPixelFormat> AVCodecWrapper::getPixelFormats() const
{
  CAST_POINTER_AND_RETURN_ARRAY_MEMBER(pix_fmts, AVPixelFormat(-1));
}

std::vector<int> AVCodecWrapper::getSupportedSamplerates() const
{
  CAST_POINTER_AND_RETURN_ARRAY_MEMBER(supported_samplerates, 0);
}

std::vector<AVSampleFormat> AVCodecWrapper::getSampleFormats() const
{
  CAST_POINTER_AND_RETURN_ARRAY_MEMBER(sample_fmts, AVSampleFormat(-1));
}

std::vector<uint64_t> AVCodecWrapper::getChannelLayouts() const
{
  CAST_POINTER_AND_RETURN_ARRAY_MEMBER(channel_layouts, uint64_t(0));
}

uint8_t AVCodecWrapper::getMaxLowres() const
{
  CAST_POINTER_AND_RETURN_MEMBER(max_lowres);
}

} // namespace ffmpeg::avcodec
