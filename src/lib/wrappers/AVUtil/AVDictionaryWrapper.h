/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/FFMpegLibrariesTypes.h>

namespace ffmpeg::avutil
{

class AVDictionaryWrapper
{
public:
  AVDictionaryWrapper() = default;
  AVDictionaryWrapper(AVDictionary *dict) : dict(dict) {}

  void          setDictionary(AVDictionary *d) { this->dict = d; }
  explicit      operator bool() const { return this->dict != nullptr; }
  AVDictionary *getDictionary() const { return this->dict; }

private:
  AVDictionary *dict{};
};

} // namespace ffmpeg::avutil
