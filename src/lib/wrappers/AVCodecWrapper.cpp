/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVCodecWrapper.h"
#include <stdexcept>

namespace LibFFmpeg
{

namespace
{

typedef struct AVCodec_56_57_58
{
  const char *               name;
  const char *               long_name;
  enum AVMediaType           type;
  enum AVCodecID             id;
  int                        capabilities;
  const AVRational *         supported_framerates;
  const enum AVPixelFormat * pix_fmts;
  const int *                supported_samplerates;
  const enum AVSampleFormat *sample_fmts;
  const uint64_t *           channel_layouts;
  uint8_t                    max_lowres;
  const AVClass *            priv_class;

  // Actually, there is more here, but nothing more of the public API
} AVCodec_56_57_58;

typedef struct AVCodec_59
{
  const char *               name;
  const char *               long_name;
  enum AVMediaType           type;
  enum AVCodecID             id;
  int                        capabilities;
  uint8_t                    max_lowres;
  const AVRational *         supported_framerates;
  const enum AVPixelFormat * pix_fmts;
  const int *                supported_samplerates;
  const enum AVSampleFormat *sample_fmts;
  const uint64_t *           channel_layouts;
  const AVClass *            priv_class;

  // Actually, there is more here, but nothing more of the public API
} AVCodec_59;

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

AVCodecID AVCodecWrapper::getCodecID()
{
  this->update();
  return this->id;
}

std::string AVCodecWrapper::getName()
{
  this->update();
  return this->name;
}
std::string AVCodecWrapper::getLongName()
{
  this->update();
  return this->long_name;
}

void AVCodecWrapper::update()
{
  if (codec == nullptr)
    return;

  if (libraryVersions.avcodec.major == 56 || //
      libraryVersions.avcodec.major == 57 || //
      libraryVersions.avcodec.major == 58)
  {
    auto p                      = reinterpret_cast<AVCodec_56_57_58 *>(codec);
    this->name                  = std::string(p->name);
    this->long_name             = std::string(p->long_name);
    this->type                  = p->type;
    this->id                    = p->id;
    this->capabilities          = p->capabilities;
    this->supported_framerates  = convertRawListToVec(p->supported_framerates, AVRational({0, 0}));
    this->pix_fmts              = convertRawListToVec(p->pix_fmts, AVPixelFormat(-1));
    this->supported_samplerates = convertRawListToVec(p->supported_samplerates, 0);
    this->sample_fmts           = convertRawListToVec(p->sample_fmts, AVSampleFormat(-1));
    this->channel_layouts       = convertRawListToVec(p->channel_layouts, uint64_t(0));
    this->max_lowres            = p->max_lowres;
  }
  else if (libraryVersions.avcodec.major == 59)
  {
    auto p                      = reinterpret_cast<AVCodec_59 *>(codec);
    this->name                  = std::string(p->name);
    this->long_name             = std::string(p->long_name);
    this->type                  = p->type;
    this->id                    = p->id;
    this->capabilities          = p->capabilities;
    this->supported_framerates  = convertRawListToVec(p->supported_framerates, AVRational({0, 0}));
    this->pix_fmts              = convertRawListToVec(p->pix_fmts, AVPixelFormat(-1));
    this->supported_samplerates = convertRawListToVec(p->supported_samplerates, 0);
    this->sample_fmts           = convertRawListToVec(p->sample_fmts, AVSampleFormat(-1));
    this->channel_layouts       = convertRawListToVec(p->channel_layouts, uint64_t(0));
    this->max_lowres            = p->max_lowres;
  }
  else
    throw std::runtime_error("Invalid library version");
}

} // namespace LibFFmpeg