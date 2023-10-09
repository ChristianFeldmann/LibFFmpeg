/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/FFMpegLibrariesTypes.h>
#include <libHandling/FFmpegLibrariesInterface.h>
#include <wrappers/AVUtil/AVDictionaryWrapper.h>

#include <memory>

namespace ffmpeg::avutil
{

class AVFrameWrapper
{
public:
  AVFrameWrapper() = default;
  AVFrameWrapper(AVFrame *frame, std::shared_ptr<FFmpegLibrariesInterface> librariesInterface);

  AVFrame *getFrame() const { return this->frame; }

  ByteVector          getData(int component) const;
  int                 getLineSize(int component) const;
  Size                getSize() const;
  int64_t             getPTS() const;
  AVPictureType       getPictType() const;
  bool                isKeyFrame() const;
  AVDictionaryWrapper getMetadata() const;

  explicit operator bool() const { return this->frame != nullptr; }

private:
  AVFrame                                  *frame{};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface{};
};

} // namespace ffmpeg::avutil
