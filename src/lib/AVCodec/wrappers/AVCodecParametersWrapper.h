/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVUtil/ColorSpace.h>
#include <AVUtil/MediaType.h>
#include <AVUtil/wrappers/AVPixFmtDescriptorConversion.h>
#include <common/InternalTypes.h>
#include <common/Types.h>
#include <libHandling/IFFmpegLibraries.h>

#include "../Channel.h"

namespace libffmpeg::avcodec
{
class AVCodecParametersWrapper
{
public:
  AVCodecParametersWrapper() = delete;
  AVCodecParametersWrapper(libffmpeg::internal::AVCodecParameters *p,
                           std::shared_ptr<IFFmpegLibraries>       libraries);

  avutil::MediaType              getCodecType() const;
  libffmpeg::internal::AVCodecID getCodecID() const;
  ByteVector                     getExtradata() const;
  Size                           getSize() const;
  avutil::ColorSpace             getColorspace() const;
  avutil::PixelFormatDescriptor  getPixelFormat() const;
  Rational                       getSampleAspectRatio() const;
  ChannelLayout                  getChannelLayout() const;

  // Set a default set of (unknown) values
  void setClearValues();

  void setAVMediaType(avutil::MediaType type);
  void setAVCodecID(libffmpeg::internal::AVCodecID id);
  void setExtradata(const ByteVector &extradata);
  void setSize(Size size);
  void setAVPixelFormat(avutil::PixelFormatDescriptor descriptor);
  void setProfileLevel(int profile, int level);
  void setSampleAspectRatio(int num, int den);

  libffmpeg::internal::AVCodecParameters *getCodecParameters() const
  {
    return this->codecParameters;
  }

private:
  libffmpeg::internal::AVCodecParameters *codecParameters{};
  std::shared_ptr<IFFmpegLibraries>       ffmpegLibraries;
};

} // namespace libffmpeg::avcodec
