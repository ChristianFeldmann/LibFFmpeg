/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/FFMpegLibrariesTypes.h>

namespace ffmpeg::avutil
{

class AVFrameWrapper
{
public:
  AVFrameWrapper() = default;
  AVFrameWrapper(AVFrame *frame, const LibraryVersions &libraryVersions);

  void clear();

  uint8_t      *getData(int component);
  int           getLineSize(int component);
  AVFrame      *getFrame() const;
  int           getWidth();
  int           getHeight();
  Size          getSize();
  int64_t       getPTS();
  AVPictureType getPictType();
  int           getKeyFrame();
  AVDictionary *getMetadata();

  explicit operator bool() const { return this->frame != nullptr; }

private:
  void update();

  // These are private. Use "update" to update them from the AVFormatContext
  uint8_t      *data[AV_NUM_DATA_POINTERS]{};
  int           linesize[AV_NUM_DATA_POINTERS]{};
  int           width{};
  int           height{};
  int           nb_samples{};
  int           format{};
  int           key_frame{};
  AVPictureType pict_type{};
  AVRational    sample_aspect_ratio{};
  int64_t       pts{};
  int64_t       pkt_pts{};
  int64_t       pkt_dts{};
  int           coded_picture_number{};
  int           display_picture_number{};
  int           quality{};
  AVDictionary *metadata{};

  AVFrame        *frame{};
  LibraryVersions libraryVersions{};
};

} // namespace ffmpeg::avutil
