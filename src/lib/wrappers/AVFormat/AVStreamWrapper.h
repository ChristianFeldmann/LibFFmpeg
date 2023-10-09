/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <libHandling/FFmpegLibrariesInterface.h>
#include <wrappers/AVCodec/AVCodecContextWrapper.h>
#include <wrappers/AVCodec/AVCodecIDWrapper.h>
#include <wrappers/AVFormat/AVCodecParametersWrapper.h>

#include <memory>

namespace ffmpeg::avformat
{

class AVStreamWrapper
{
public:
  AVStreamWrapper() {}
  AVStreamWrapper(AVStream *src_str, std::shared_ptr<FFmpegLibrariesInterface> librariesInterface);

  explicit operator bool() const { return this->stream != nullptr; };

  AVMediaType                     getCodecType();
  std::string                     getCodecTypeName();
  AVCodecID                       getCodecID();
  avcodec::AVCodecContextWrapper &getCodec();
  AVRational                      getAvgFrameRate();
  AVRational                      getTimeBase();
  Size                            getFrameSize();
  AVColorSpace                    getColorspace();
  AVPixelFormat                   getPixelFormat();
  ByteVector                      getExtradata();
  int                             getIndex();
  AVCodecParametersWrapper        getCodecpar();

private:
  void update();

  // These are private. Use "update" to update them from the AVFormatContext
  int                            index{};
  int                            id{};
  avcodec::AVCodecContextWrapper codec{};
  AVRational                     time_base{};
  int64_t                        start_time{};
  int64_t                        duration{};
  int64_t                        nb_frames{};
  int64_t                        disposition{};
  enum AVDiscard                 discard
  {
  };
  AVRational sample_aspect_ratio{};
  AVRational avg_frame_rate{};
  int        nb_side_data{};
  int        event_flags{};

  // The AVCodecParameters are present from avformat major version 57 and up.
  AVCodecParametersWrapper codecpar{};

  AVStream                                 *stream{};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface{};
};

} // namespace ffmpeg::avformat