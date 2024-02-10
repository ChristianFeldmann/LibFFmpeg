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
using ffmpeg::internal::AVDictionary;

class AVDictionaryWrapper
{
public:
  AVDictionaryWrapper() = delete;
  AVDictionaryWrapper(AVDictionary *dictionary, std::shared_ptr<IFFmpegLibraries> ffmpegLibraries);

  explicit      operator bool() const { return this->dictionary != nullptr; }
  AVDictionary *getDictionary() const { return this->dictionary; }

  DictionaryMap toMap() const;

private:
  AVDictionary *                    dictionary{};
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries{};
};

} // namespace ffmpeg::avutil
