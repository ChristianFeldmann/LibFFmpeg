/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVFrameWrapper.h"

#include "AVFrameWrapperInternal.h"
#include "CastUtilClasses.h"

#include <common/InternalTypes.h>

#include <stdexcept>

namespace ffmpeg::avutil
{

namespace
{

using ffmpeg::internal::AVDictionary;
using ffmpeg::internal::AVFrame;
using ffmpeg::internal::AVPictureType;
using ffmpeg::internal::AVRational;

ByteVector copyFrameDataFromRawArray(const uint8_t *inputData, Size size, const int linesize)
{
  ByteVector data;
  data.resize(size.height * size.width);

  auto inputDataPtrAsBytes = reinterpret_cast<const std::byte *>(inputData);
  auto outputIterator      = data.begin();

  for (int y = 0; y < size.height; ++y)
  {
    std::copy(inputDataPtrAsBytes, inputDataPtrAsBytes + size.width, outputIterator);
    inputDataPtrAsBytes += linesize;
    outputIterator += size.width;
  }

  return data;
}

} // namespace

AVFrameWrapper::AVFrameWrapper(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
    : ffmpegLibraries(ffmpegLibraries)
{
  if (!ffmpegLibraries)
    throw std::runtime_error("Provided ffmpeg libraries pointer must not be null");

  this->frame = std::unique_ptr<AVFrame, AVFrameDeleter>(
      reinterpret_cast<AVFrame *>(this->ffmpegLibraries->avutil.av_frame_alloc()),
      AVFrameDeleter(this->ffmpegLibraries));

  if (!this->frame)
    throw std::runtime_error("Error allocating AVFrame");
}

AVFrameWrapper::AVFrameWrapper(AVFrameWrapper &&other)
{
  this->frame           = std::move(other.frame);
  this->ffmpegLibraries = std::move(other.ffmpegLibraries);
}

AVFrameWrapper &AVFrameWrapper::operator=(AVFrameWrapper &&other)
{
  this->frame           = std::move(other.frame);
  this->ffmpegLibraries = std::move(other.ffmpegLibraries);
  return *this;
}

ByteVector AVFrameWrapper::getData(int component) const
{
  if (component < 0 || component > AV_NUM_DATA_POINTERS)
    return {};

  uint8_t *dataPointer;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame.get(), dataPointer, data[component]);

  int linesize;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame.get(), linesize, linesize[component]);

  const auto componentSize =
      getSizeOfFrameComponent(component, this->getSize(), this->getPixelFormatDescriptor());
  const auto dataSize = componentSize.width * componentSize.height;

  return copyFrameDataFromRawArray(dataPointer, componentSize, linesize);
}

int AVFrameWrapper::getLineSize(int component) const
{
  if (component < 0 || component > AV_NUM_DATA_POINTERS)
    return {};

  int linesize;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame.get(), linesize, linesize[component]);
  return linesize;
}

Size AVFrameWrapper::getSize() const
{
  int width;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame.get(), width, width);

  int height;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame.get(), height, height);

  return {width, height};
}

std::optional<int64_t> AVFrameWrapper::getPTS() const
{
  int64_t pts;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame.get(), pts, pts);

  constexpr int64_t AV_NOPTS_VALUE = 0x8000000000000000;
  if (pts == AV_NOPTS_VALUE)
    return {};
  return pts;
}

avutil::PictureType AVFrameWrapper::getPictType() const
{
  AVPictureType pictureType;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame.get(), pictureType, pict_type);
  return ffmpeg::avutil::toPictureType(pictureType);
}

bool AVFrameWrapper::isKeyFrame() const
{
  int keyframe;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame.get(), keyframe, key_frame);
  return keyframe == 1;
}

std::optional<AVDictionaryWrapper> AVFrameWrapper::getMetadata() const
{
  const auto version = this->ffmpegLibraries->getLibrariesVersion().avutil.major;

  if (version == 57 || version == 58)
  {
    const auto p = reinterpret_cast<internal::avutil::AVFrame_57 *>(this->frame.get());
    if (p->metadata)
      return AVDictionaryWrapper(p->metadata, this->ffmpegLibraries);
  }

  return {};
}

PixelFormatDescriptor AVFrameWrapper::getPixelFormatDescriptor() const
{
  int format;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame.get(), format, format);

  return convertAVPixFmtDescriptor(static_cast<ffmpeg::internal::AVPixelFormat>(format),
                                   this->ffmpegLibraries);
}

Rational AVFrameWrapper::getSampleAspectRatio() const
{
  AVRational sampleAspectRatio;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame.get(), sampleAspectRatio, sample_aspect_ratio);

  return Rational({sampleAspectRatio.num, sampleAspectRatio.den});
}

void AVFrameWrapper::AVFrameDeleter::operator()(AVFrame *frame) const noexcept
{
  if (frame != nullptr)
    this->ffmpegLibraries->avutil.av_frame_free(&frame);
}

} // namespace ffmpeg::avutil
