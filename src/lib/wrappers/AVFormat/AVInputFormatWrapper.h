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
  AVInputFormatWrapper(AVInputFormat                            *inputFormat,
                       std::shared_ptr<FFmpegLibrariesInterface> librariesInterface);

  std::string getName() const;

  explicit operator bool() const { return this->inputFormat != nullptr; };

private:
  AVInputFormat                            *inputFormat{};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface{};
};

} // namespace ffmpeg::avformat
