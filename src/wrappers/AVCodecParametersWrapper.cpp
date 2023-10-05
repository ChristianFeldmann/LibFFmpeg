/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVCodecParametersWrapper.h"

#include "Functions.h"
namespace LibFFmpeg
{

namespace
{

// AVCodecParameters is part of avcodec.
typedef struct AVCodecParameters_57_58_59_60
{
  AVMediaType                   codec_type;
  AVCodecID                     codec_id;
  uint32_t                      codec_tag;
  uint8_t *                     extradata;
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
} AVCodecParameters_57_58_59_60;

} // namespace

AVCodecParametersWrapper::AVCodecParametersWrapper(AVCodecParameters *    p,
                                                   const LibraryVersions &libraryVersions)
{
  this->param           = p;
  this->libraryVersions = libraryVersions;
  this->update();
}

AVMediaType AVCodecParametersWrapper::getCodecType()
{
  this->update();
  return this->codec_type;
}

AVCodecID AVCodecParametersWrapper::getCodecID()
{
  this->update();
  return this->codec_id;
}

ByteVector AVCodecParametersWrapper::getExtradata()
{
  this->update();
  return this->extradata;
}

Size AVCodecParametersWrapper::getSize()
{
  this->update();
  return Size({this->width, this->height});
}

AVColorSpace AVCodecParametersWrapper::getColorspace()
{
  this->update();
  return this->color_space;
}

AVPixelFormat AVCodecParametersWrapper::getPixelFormat()
{
  this->update();
  return AVPixelFormat(this->format);
}

Ratio AVCodecParametersWrapper::getSampleAspectRatio()
{
  this->update();
  return {this->sample_aspect_ratio.num, this->sample_aspect_ratio.den};
}

void AVCodecParametersWrapper::setClearValues()
{
  if (this->libraryVersions.avformat.major == 57 || //
      this->libraryVersions.avformat.major == 58 || //
      this->libraryVersions.avformat.major == 59 || //
      this->libraryVersions.avformat.major == 60)
  {
    auto p                   = reinterpret_cast<AVCodecParameters_57_58_59_60 *>(this->param);
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
  this->update();
}

void AVCodecParametersWrapper::setAVMediaType(AVMediaType type)
{
  if (this->libraryVersions.avformat.major == 57 || //
      this->libraryVersions.avformat.major == 58 || //
      this->libraryVersions.avformat.major == 59 || //
      this->libraryVersions.avformat.major == 60)
  {
    auto p           = reinterpret_cast<AVCodecParameters_57_58_59_60 *>(this->param);
    p->codec_type    = type;
    this->codec_type = type;
  }
}

void AVCodecParametersWrapper::setAVCodecID(AVCodecID id)
{
  if (this->libraryVersions.avformat.major == 57 || //
      this->libraryVersions.avformat.major == 58 || //
      this->libraryVersions.avformat.major == 59 || //
      this->libraryVersions.avformat.major == 60)
  {
    auto p         = reinterpret_cast<AVCodecParameters_57_58_59_60 *>(this->param);
    p->codec_id    = id;
    this->codec_id = id;
  }
}

void AVCodecParametersWrapper::setExtradata(const ByteVector &data)
{
  if (this->libraryVersions.avformat.major == 57 || //
      this->libraryVersions.avformat.major == 58 || //
      this->libraryVersions.avformat.major == 59 || //
      this->libraryVersions.avformat.major == 60)
  {
    this->extradata   = data;
    auto p            = reinterpret_cast<AVCodecParameters_57_58_59_60 *>(this->param);
    p->extradata      = reinterpret_cast<uint8_t *>(this->extradata.data());
    p->extradata_size = static_cast<int>(this->extradata.size());
  }
}

void AVCodecParametersWrapper::setSize(Size size)
{
  if (this->libraryVersions.avformat.major == 57 || //
      this->libraryVersions.avformat.major == 58 || //
      this->libraryVersions.avformat.major == 59 || //
      this->libraryVersions.avformat.major == 60)
  {
    auto p       = reinterpret_cast<AVCodecParameters_57_58_59_60 *>(this->param);
    p->width     = size.width;
    p->height    = size.height;
    this->width  = size.width;
    this->height = size.height;
  }
}

void AVCodecParametersWrapper::setAVPixelFormat(AVPixelFormat format)
{
  if (this->libraryVersions.avformat.major == 57 || //
      this->libraryVersions.avformat.major == 58 || //
      this->libraryVersions.avformat.major == 59 || //
      this->libraryVersions.avformat.major == 60)
  {
    auto p       = reinterpret_cast<AVCodecParameters_57_58_59_60 *>(this->param);
    p->format    = format;
    this->format = format;
  }
}

void AVCodecParametersWrapper::setProfileLevel(int profile, int level)
{
  if (this->libraryVersions.avformat.major == 57 || //
      this->libraryVersions.avformat.major == 58 || //
      this->libraryVersions.avformat.major == 59 || //
      this->libraryVersions.avformat.major == 60)
  {
    auto p        = reinterpret_cast<AVCodecParameters_57_58_59_60 *>(this->param);
    p->profile    = profile;
    p->level      = level;
    this->profile = profile;
    this->level   = level;
  }
}

void AVCodecParametersWrapper::setSampleAspectRatio(int num, int den)
{
  if (this->libraryVersions.avformat.major == 57 || //
      this->libraryVersions.avformat.major == 58 || //
      this->libraryVersions.avformat.major == 59 || //
      this->libraryVersions.avformat.major == 60)
  {
    auto       p = reinterpret_cast<AVCodecParameters_57_58_59_60 *>(param);
    AVRational ratio;
    ratio.num                 = num;
    ratio.den                 = den;
    p->sample_aspect_ratio    = ratio;
    this->sample_aspect_ratio = ratio;
  }
}

void AVCodecParametersWrapper::update()
{
  if (this->param == nullptr)
    return;

  if (this->libraryVersions.avformat.major == 56)
  {
    // This data structure does not exist in avformat major version 56.
    this->param = nullptr;
  }
  else if (this->libraryVersions.avformat.major == 57 || //
           this->libraryVersions.avformat.major == 58 || //
           this->libraryVersions.avformat.major == 59 || //
           this->libraryVersions.avformat.major == 60)
  {
    auto p = reinterpret_cast<AVCodecParameters_57_58_59_60 *>(this->param);

    this->codec_type            = p->codec_type;
    this->codec_id              = p->codec_id;
    this->codec_tag             = p->codec_tag;
    this->extradata             = copyDataFromRawArray(p->extradata, p->extradata_size);
    this->format                = p->format;
    this->bit_rate              = p->bit_rate;
    this->bits_per_coded_sample = p->bits_per_coded_sample;
    this->bits_per_raw_sample   = p->bits_per_raw_sample;
    this->profile               = p->profile;
    this->level                 = p->level;
    this->width                 = p->width;
    this->height                = p->height;
    this->sample_aspect_ratio   = p->sample_aspect_ratio;
    this->field_order           = p->field_order;
    this->color_range           = p->color_range;
    this->color_primaries       = p->color_primaries;
    this->color_trc             = p->color_trc;
    this->color_space           = p->color_space;
    this->chroma_location       = p->chroma_location;
    this->video_delay           = p->video_delay;
  }
  else
    throw std::runtime_error("Invalid library version");
}

} // namespace LibFFmpeg
