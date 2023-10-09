/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <libHandling/FFmpegLibrariesInterface.h>

namespace ffmpeg::avformat
{

class AVInputFormatWrapper
{
public:
  AVInputFormatWrapper() = default;
  AVInputFormatWrapper(AVInputFormat                            *avInputFormat,
                       std::shared_ptr<FFmpegLibrariesInterface> librariesInterface);

  std::string getName();

  explicit operator bool() const { return this->avInputFormat != nullptr; };

private:
  // Update all private values from the AVCodecContext
  void update();

  // These are here for debugging purposes.
  std::string name{};
  std::string long_name{};
  int         flags{};
  std::string extensions{};
  std::string mime_type{};

  AVInputFormat                            *avInputFormat{};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface{};
};

} // namespace ffmpeg::avformat
