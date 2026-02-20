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
  ~AVFrameWrapper()                      = default;
  AVFrameWrapper(const AVFrameWrapper &) = delete;
  AVFrameWrapper(AVFrameWrapper &&frame) noexcept;
  AVFrameWrapper(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);

  AVFrameWrapper &operator=(AVFrameWrapper &&) noexcept;
  AVFrameWrapper &operator=(const AVFrameWrapper &) = delete;

  [[nodiscard]] libffmpeg::internal::AVFrame *getFrame() const { return this->frame.get(); }

  [[nodiscard]] ByteVector                         getData(int component) const;
  [[nodiscard]] int                                getLineSize(int component) const;
  [[nodiscard]] Size                               getSize() const;
  [[nodiscard]] std::optional<int64_t>             getPTS() const;
  [[nodiscard]] avutil::PictureType                getPictType() const;
  [[nodiscard]] bool                               isKeyFrame() const;
  [[nodiscard]] std::optional<AVDictionaryWrapper> getMetadata() const;
  [[nodiscard]] PixelFormatDescriptor              getPixelFormatDescriptor() const;
  [[nodiscard]] Rational                           getSampleAspectRatio() const;

  explicit operator bool() const { return this->frame != nullptr; }

private:
  class AVFrameDeleter
  {
  public:
    AVFrameDeleter() = default;
    AVFrameDeleter(const std::shared_ptr<IFFmpegLibraries> &ffmpegLibraries)
        : ffmpegLibraries(ffmpegLibraries) {};
    void operator()(libffmpeg::internal::AVFrame *frame) const noexcept;

  private:
    std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};
  };

  std::unique_ptr<libffmpeg::internal::AVFrame, AVFrameDeleter> frame{nullptr, AVFrameDeleter()};
  std::shared_ptr<IFFmpegLibraries>                             ffmpegLibraries{};
};

} // namespace libffmpeg::avutil
