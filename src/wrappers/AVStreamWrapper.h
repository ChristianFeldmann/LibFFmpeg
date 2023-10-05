/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include "AVCodecContextWrapper.h"
#include "AVCodecIDWrapper.h"
#include "AVCodecParametersWrapper.h"

#include <common/FFMpegLibrariesTypes.h>

namespace LibFFmpeg
{

class AVStreamWrapper
{
public:
  AVStreamWrapper() {}
  AVStreamWrapper(AVStream *src_str, const LibraryVersions &libraryVersions);

  explicit operator bool() const { return this->stream != nullptr; };

  AVMediaType              getCodecType();
  std::string              getCodecTypeName();
  AVCodecID                getCodecID();
  AVCodecContextWrapper &  getCodec();
  AVRational               getAvgFrameRate();
  AVRational               getTimeBase();
  Size                     getFrameSize();
  AVColorSpace             getColorspace();
  AVPixelFormat            getPixelFormat();
  ByteVector               getExtradata();
  int                      getIndex();
  AVCodecParametersWrapper getCodecpar();

private:
  void update();

  // These are private. Use "update" to update them from the AVFormatContext
  int                   index{};
  int                   id{};
  AVCodecContextWrapper codec{};
  AVRational            time_base{};
  int64_t               start_time{};
  int64_t               duration{};
  int64_t               nb_frames{};
  int                   disposition{};
  enum AVDiscard        discard
  {
  };
  AVRational sample_aspect_ratio{};
  AVRational avg_frame_rate{};
  int        nb_side_data{};
  int        event_flags{};

  // The AVCodecParameters are present from avformat major version 57 and up.
  AVCodecParametersWrapper codecpar{};

  AVStream *      stream{};
  LibraryVersions libraryVersions{};
};

} // namespace LibFFmpeg