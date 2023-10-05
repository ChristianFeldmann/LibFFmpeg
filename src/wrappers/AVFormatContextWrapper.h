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

#include "AVDictionaryWrapper.h"
#include "AVInputFormatWrapper.h"
#include "AVPacketWrapper.h"
#include "AVStreamWrapper.h"

#include <common/FFMpegLibrariesTypes.h>

namespace LibFFmpeg
{

class AVFormatContextWrapper
{
public:
  AVFormatContextWrapper() = default;
  AVFormatContextWrapper(AVFormatContext *c, const LibraryVersions &libraryVersions);

  void     updateFrom(AVFormatContext *c);
  explicit operator bool() const;

  unsigned int         getNbStreams();
  AVStreamWrapper      getStream(int idx);
  AVInputFormatWrapper getInputFormat();
  int64_t              getStartTime();
  int64_t              getDuration();
  AVFormatContext *    getFormatCtx() const;
  AVDictionaryWrapper  getMetadata();

private:
  // Update all private values from the AVFormatContext
  void update();

  AVInputFormatWrapper iformat{};

  // These are private. Use "update" to update them from the AVFormatContext
  int                          ctx_flags{0};
  unsigned int                 nb_streams{0};
  std::vector<AVStreamWrapper> streams;
  std::string                  filename{};
  int64_t                      start_time{-1};
  int64_t                      duration{-1};
  int                          bit_rate{0};
  unsigned int                 packet_size{0};
  int                          max_delay{0};
  int                          flags{0};

  unsigned int        probesize{0};
  int                 max_analyze_duration{0};
  std::string         key{};
  unsigned int        nb_programs{0};
  AVCodecID           video_codec_id{AV_CODEC_ID_NONE};
  AVCodecID           audio_codec_id{AV_CODEC_ID_NONE};
  AVCodecID           subtitle_codec_id{AV_CODEC_ID_NONE};
  unsigned int        max_index_size{0};
  unsigned int        max_picture_buffer{0};
  unsigned int        nb_chapters{0};
  AVDictionaryWrapper metadata;

  AVFormatContext *ctx{nullptr};
  LibraryVersions  libraryVersions{};
};

} // namespace LibFFmpeg
