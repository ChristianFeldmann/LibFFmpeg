/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <libHandling/FFmpegLibrariesInterface.h>

namespace ffmpeg::avformat
{

struct AVInputFormatFlags
{
  bool noFile{};
  bool needNumber{};
  bool experimental{};
  bool showIDs{};
  bool globalHeaders{};
  bool noTiemstamps{};
  bool genericIndex{};
  bool tsDiscont{};
  bool variableFPS{};
  bool noDimensions{};
  bool noStreams{};
  bool noBinSearch{};
  bool noGenSearch{};
  bool noByteSeek{};
  bool allowFlush{};
  bool tsNonStrict{};
  bool tsNegative{};
  bool seekToPTS{};
};

std::string to_string(const AVInputFormatFlags &flags);

class AVInputFormatWrapper
{
public:
  AVInputFormatWrapper() = default;
  AVInputFormatWrapper(AVInputFormat                            *inputFormat,
                       std::shared_ptr<FFmpegLibrariesInterface> librariesInterface);

  std::string        getName() const;
  std::string        getLongName() const;
  AVInputFormatFlags getFlags() const;
  std::string        getExtensions() const;
  std::string        getMimeType() const;

  explicit operator bool() const { return this->inputFormat != nullptr; };

private:
  AVInputFormat                            *inputFormat{};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface{};
};

} // namespace ffmpeg::avformat
