/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <libHandling/IFFmpegLibraries.h>

namespace libffmpeg::avformat
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
bool        operator==(const AVInputFormatFlags &lhs, const AVInputFormatFlags &rhs);

class AVInputFormatWrapper
{
public:
  AVInputFormatWrapper() = delete;
  AVInputFormatWrapper(libffmpeg::internal::AVInputFormat *inputFormat,
                       std::shared_ptr<IFFmpegLibraries>   ffmpegLibraries);

  [[nodiscard]] std::string        getName() const;
  [[nodiscard]] std::string        getLongName() const;
  [[nodiscard]] AVInputFormatFlags getFlags() const;
  [[nodiscard]] std::string        getExtensions() const;
  [[nodiscard]] std::string        getMimeType() const;

  explicit operator bool() const { return this->inputFormat != nullptr; };

private:
  libffmpeg::internal::AVInputFormat *inputFormat{};
  std::shared_ptr<IFFmpegLibraries>   ffmpegLibraries{};
};

} // namespace libffmpeg::avformat
