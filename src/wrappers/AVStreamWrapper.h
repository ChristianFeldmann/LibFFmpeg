/* LibFFmpeg++
 * Copyright (C) 2023 Christian Feldmann [christian.feldmann@gmx.de]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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