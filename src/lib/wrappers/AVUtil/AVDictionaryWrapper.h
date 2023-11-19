/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/FFMpegLibrariesTypes.h>
#include <libHandling/FFmpegLibrariesInterface.h>

#include <map>

namespace ffmpeg::avutil
{

using DictionaryMap = std::map<std::string, std::string>;

class AVDictionaryWrapper
{
public:
  AVDictionaryWrapper() = default;
  AVDictionaryWrapper(AVDictionary *                            dict,
                      std::shared_ptr<FFmpegLibrariesInterface> librariesInterface);

  explicit      operator bool() const { return this->dict != nullptr; }
  AVDictionary *getDictionary() const { return this->dict; }

  DictionaryMap toMap() const;

private:
  AVDictionary *                            dict{};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface{};
};

} // namespace ffmpeg::avutil
