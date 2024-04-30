/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVUtil/PictureType.h>
#include <AVUtil/wrappers/AVDictionaryWrapper.h>
#include <AVUtil/wrappers/AVPixFmtDescriptorConversion.h>
#include <common/Types.h>
#include <libHandling/IFFmpegLibraries.h>

#include <memory>

namespace libffmpeg::avutil
{

class AVFrameWrapper
{
public:
  AVFrameWrapper()                       = delete;
  AVFrameWrapper(const AVFrameWrapper &) = delete;
  AVFrameWrapper(AVFrameWrapper &&frame);
  AVFrameWrapper(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);

  AVFrameWrapper &operator=(AVFrameWrapper &&);
  AVFrameWrapper &operator=(const AVFrameWrapper &) = delete;

  libffmpeg::internal::AVFrame *getFrame() const { return this->frame.get(); }

  ByteVector                         getData(int component) const;
  int                                getLineSize(int component) const;
  Size                               getSize() const;
  std::optional<int64_t>             getPTS() const;
  avutil::PictureType                getPictType() const;
  bool                               isKeyFrame() const;
  std::optional<AVDictionaryWrapper> getMetadata() const;
  PixelFormatDescriptor              getPixelFormatDescriptor() const;
  Rational                           getSampleAspectRatio() const;

  explicit operator bool() const { return this->frame != nullptr; }

private:
  class AVFrameDeleter
  {
  public:
    AVFrameDeleter() = default;
    AVFrameDeleter(const std::shared_ptr<IFFmpegLibraries> &ffmpegLibraries)
        : ffmpegLibraries(ffmpegLibraries){};
    void operator()(libffmpeg::internal::AVFrame *frame) const noexcept;

  private:
    std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};
  };

  std::unique_ptr<libffmpeg::internal::AVFrame, AVFrameDeleter> frame{nullptr, AVFrameDeleter()};
  std::shared_ptr<IFFmpegLibraries>                             ffmpegLibraries{};
};

} // namespace libffmpeg::avutil
