/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVDictionaryWrapper.h"

namespace ffmpeg::avutil
{

constexpr auto AV_DICT_IGNORE_SUFFIX = 2;

AVDictionaryWrapper::AVDictionaryWrapper(
    AVDictionary *dict, std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
    : dict(dict), librariesInterface(librariesInterface)
{
}

DictionaryMap AVDictionaryWrapper::toMap() const
{
  if (this->dict == nullptr)
    return {};

  DictionaryMap map;

  AVDictionaryEntry *entry{};
  while (entry = this->librariesInterface->avutil.av_dict_get(
             this->dict, "", entry, AV_DICT_IGNORE_SUFFIX))
  {
    const auto key   = std::string(entry->key);
    const auto value = std::string(entry->value);
    if (!key.empty() || value.empty())
      map[key] = value;
  }

  return map;
}

} // namespace ffmpeg::avutil
