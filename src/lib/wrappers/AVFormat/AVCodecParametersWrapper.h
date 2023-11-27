/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/ColorTypes.h>
#include <common/InternalTypes.h>
#include <common/Types.h>
#include <libHandling/FFmpegLibrariesInterface.h>
#include <wrappers/AVUtil/AVPixFmtDescriptorWrapper.h>

namespace ffmpeg::avformat
{
class AVCodecParametersWrapper
{
public:
  AVCodecParametersWrapper() = default;
  AVCodecParametersWrapper(ffmpeg::internal::AVCodecParameters      *p,
                           std::shared_ptr<FFmpegLibrariesInterface> libraries);
  explicit operator bool() const { return this->codecParameters != nullptr; }

  MediaType                         getCodecType() const;
  ffmpeg::internal::AVCodecID       getCodecID() const;
  ByteVector                        getExtradata() const;
  Size                              getSize() const;
  ColorSpace                        getColorspace() const;
  avutil::AVPixFmtDescriptorWrapper getPixelFormat() const;
  Rational                          getSampleAspectRatio() const;

  // Set a default set of (unknown) values
  void setClearValues();

  void setAVMediaType(MediaType type);
  void setAVCodecID(ffmpeg::internal::AVCodecID id);
  void setExtradata(const ByteVector &extradata);
  void setSize(Size size);
  void setAVPixelFormat(avutil::AVPixFmtDescriptorWrapper descriptor);
  void setProfileLevel(int profile, int level);
  void setSampleAspectRatio(int num, int den);

  ffmpeg::internal::AVCodecParameters *getCodecParameters() const { return this->codecParameters; }

private:
  ffmpeg::internal::AVCodecParameters      *codecParameters{};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface;
};

} // namespace ffmpeg::avformat
