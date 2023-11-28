/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <libHandling/IFFmpegLibraries.h>

#include <map>

namespace ffmpeg::avutil
{

using DictionaryMap = std::map<std::string, std::string>;

class AVDictionaryWrapper
{
public:
  AVDictionaryWrapper() = default;
  AVDictionaryWrapper(ffmpeg::internal::AVDictionary   *dict,
                      std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);

  explicit                        operator bool() const { return this->dict != nullptr; }
  ffmpeg::internal::AVDictionary *getDictionary() const { return this->dict; }

  DictionaryMap toMap() const;

private:
  ffmpeg::internal::AVDictionary   *dict{};
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};
};

} // namespace ffmpeg::avutil
