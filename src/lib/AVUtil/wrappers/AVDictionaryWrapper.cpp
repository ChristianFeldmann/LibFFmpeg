/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVDictionaryWrapper.h"

namespace ffmpeg::avutil
{

using ffmpeg::internal::AVDictionary;
using ffmpeg::internal::AVDictionaryEntry;

constexpr auto AV_DICT_IGNORE_SUFFIX = 2;

AVDictionaryWrapper::AVDictionaryWrapper(AVDictionary                     *dict,
                                         std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
    : dict(dict), ffmpegLibraries(ffmpegLibraries)
{
}

DictionaryMap AVDictionaryWrapper::toMap() const
{
  if (this->dict == nullptr)
    return {};

  DictionaryMap map;

  AVDictionaryEntry *entry{};
  while (true)
  {
    entry = this->ffmpegLibraries->avutil.av_dict_get(this->dict, "", entry, AV_DICT_IGNORE_SUFFIX);
    if (entry == nullptr)
      break;

    const auto key   = std::string(entry->key);
    const auto value = std::string(entry->value);
    if (!key.empty() || value.empty())
      map[key] = value;
  }

  return map;
}

} // namespace ffmpeg::avutil
