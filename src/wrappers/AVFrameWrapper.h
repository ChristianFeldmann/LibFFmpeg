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

#include <common/FFMpegLibrariesTypes.h>

namespace LibFFmpeg
{

class AVFrameWrapper
{
public:
  AVFrameWrapper() = default;
  AVFrameWrapper(AVFrame *frame, const LibraryVersions &libraryVersions);

  void clear();

  uint8_t *     getData(int component);
  int           getLineSize(int component);
  AVFrame *     getFrame() const;
  int           getWidth();
  int           getHeight();
  Size          getSize();
  int           getPTS();
  AVPictureType getPictType();
  int           getKeyFrame();
  AVDictionary *getMetadata();

  explicit operator bool() const { return this->frame != nullptr; }

private:
  void update();

  // These are private. Use "update" to update them from the AVFormatContext
  uint8_t *     data[AV_NUM_DATA_POINTERS]{};
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

  AVFrame *       frame{};
  LibraryVersions libraryVersions{};
};

} // namespace LibFFmpeg
