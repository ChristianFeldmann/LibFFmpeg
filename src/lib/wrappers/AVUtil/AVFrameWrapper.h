/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/Types.h>
#include <libHandling/IFFmpegLibraries.h>
#include <wrappers/AVUtil/AVDictionaryWrapper.h>
#include <wrappers/AVUtil/AVPixFmtDescriptorConversion.h>

#include <memory>

namespace ffmpeg::avutil
{

class AVFrameWrapper
{
public:
  AVFrameWrapper()                       = delete;
  AVFrameWrapper(const AVFrameWrapper &) = delete;
  AVFrameWrapper(AVFrameWrapper &&frame);
  AVFrameWrapper(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);
  ~AVFrameWrapper();

  ffmpeg::internal::AVFrame *getFrame() const { return this->frame; }

  ByteVector            getData(int component) const;
  int                   getLineSize(int component) const;
  Size                  getSize() const;
  int64_t               getPTS() const;
  PictureType           getPictType() const;
  bool                  isKeyFrame() const;
  AVDictionaryWrapper   getMetadata() const;
  PixelFormatDescriptor getPixelFormatDescriptor() const;
  Rational              getSampleAspectRatio() const;

  explicit operator bool() const { return this->frame != nullptr; }

private:
  ffmpeg::internal::AVFrame        *frame{};
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};
};

} // namespace ffmpeg::avutil
