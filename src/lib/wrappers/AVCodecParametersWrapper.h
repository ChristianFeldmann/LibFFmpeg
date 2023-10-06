/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <libHandling/FFmpegLibrariesInterface.h>

namespace LibFFmpeg
{
class AVCodecParametersWrapper
{
public:
  AVCodecParametersWrapper() = default;
  AVCodecParametersWrapper(AVCodecParameters                        *p,
                           std::shared_ptr<FFmpegLibrariesInterface> libraries);
  explicit operator bool() const { return this->param != nullptr; }

  AVMediaType   getCodecType();
  AVCodecID     getCodecID();
  ByteVector    getExtradata();
  Size          getSize();
  AVColorSpace  getColorspace();
  AVPixelFormat getPixelFormat();
  Ratio         getSampleAspectRatio();

  // Set a default set of (unknown) values
  void setClearValues();

  void setAVMediaType(AVMediaType type);
  void setAVCodecID(AVCodecID id);
  void setExtradata(const ByteVector &extradata);
  void setSize(Size size);
  void setAVPixelFormat(AVPixelFormat f);
  void setProfileLevel(int profile, int level);
  void setSampleAspectRatio(int num, int den);

  AVCodecParameters *getCodecParameters() const { return this->param; }

private:
  // Update all private values from the AVCodecParameters
  void update();

  // These are private. Use "update" to update them from the AVCodecParameters
  AVMediaType                   codec_type{};
  AVCodecID                     codec_id{};
  uint32_t                      codec_tag{};
  ByteVector                    extradata{};
  int                           format{};
  int64_t                       bit_rate{};
  int                           bits_per_coded_sample{};
  int                           bits_per_raw_sample{};
  int                           profile{};
  int                           level{};
  int                           width{};
  int                           height{};
  AVRational                    sample_aspect_ratio{};
  AVFieldOrder                  field_order{};
  AVColorRange                  color_range{};
  AVColorPrimaries              color_primaries{};
  AVColorTransferCharacteristic color_trc{};
  AVColorSpace                  color_space{};
  AVChromaLocation              chroma_location{};
  int                           video_delay{};

  AVCodecParameters                        *param{};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface;
};

} // namespace LibFFmpeg
