/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include "AVDictionaryWrapper.h"
#include "AVInputFormatWrapper.h"
#include "AVPacketWrapper.h"
#include "AVStreamWrapper.h"

#include <libHandling/FFmpegLibrariesInterface.h>

#include <memory>

namespace LibFFmpeg
{

class AVFormatContextWrapper
{
public:
  AVFormatContextWrapper() = default;
  AVFormatContextWrapper(std::shared_ptr<FFmpegLibrariesInterface> librariesInterface);

  ResultAndLog openFile(std::filesystem::path path);

  explicit operator bool() const;

  unsigned int         getNbStreams();
  AVStreamWrapper      getStream(int idx);
  AVInputFormatWrapper getInputFormat();
  int64_t              getStartTime();
  int64_t              getDuration();
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
  int64_t                      bit_rate{0};
  unsigned int                 packet_size{0};
  int                          max_delay{0};
  int                          flags{0};

  int64_t             probesize{0};
  int64_t             max_analyze_duration{0};
  std::string         key{};
  unsigned int        nb_programs{0};
  AVCodecID           video_codec_id{AV_CODEC_ID_NONE};
  AVCodecID           audio_codec_id{AV_CODEC_ID_NONE};
  AVCodecID           subtitle_codec_id{AV_CODEC_ID_NONE};
  unsigned int        max_index_size{0};
  unsigned int        max_picture_buffer{0};
  unsigned int        nb_chapters{0};
  AVDictionaryWrapper metadata;

  AVFormatContext                          *formatContext{nullptr};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface{};
};

} // namespace LibFFmpeg
