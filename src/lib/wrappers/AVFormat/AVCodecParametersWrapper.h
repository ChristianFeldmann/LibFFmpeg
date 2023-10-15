/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/ColorTypes.h>
#include <libHandling/FFmpegLibrariesInterface.h>
#include <wrappers/AVUtil/AVPixFmtDescriptorWrapper.h>

namespace ffmpeg::avformat
{
class AVCodecParametersWrapper
{
public:
  AVCodecParametersWrapper() = default;
  AVCodecParametersWrapper(AVCodecParameters                        *p,
                           std::shared_ptr<FFmpegLibrariesInterface> libraries);
  explicit operator bool() const { return this->codecParameters != nullptr; }

  AVMediaType                       getCodecType() const;
  AVCodecID                         getCodecID() const;
  ByteVector                        getExtradata() const;
  Size                              getSize() const;
  ColorSpace                        getColorspace() const;
  avutil::AVPixFmtDescriptorWrapper getPixelFormat() const;
  Ratio                             getSampleAspectRatio() const;

  // Set a default set of (unknown) values
  void setClearValues();

  void setAVMediaType(AVMediaType type);
  void setAVCodecID(AVCodecID id);
  void setExtradata(const ByteVector &extradata);
  void setSize(Size size);
  void setAVPixelFormat(avutil::AVPixFmtDescriptorWrapper descriptor);
  void setProfileLevel(int profile, int level);
  void setSampleAspectRatio(int num, int den);

  AVCodecParameters *getCodecParameters() const { return this->codecParameters; }

private:
  AVCodecParameters                        *codecParameters{};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface;
};

} // namespace ffmpeg::avformat
