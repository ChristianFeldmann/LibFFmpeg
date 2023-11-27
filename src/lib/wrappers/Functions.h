/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

namespace ffmpeg
{

inline ByteVector copyDataFromRawArray(const uint8_t *inputData, const int inputDataSize)
{
  if (inputDataSize <= 0)
    return {};

  ByteVector data;
  data.resize(inputDataSize);

  const auto inputDataAsBytes = reinterpret_cast<const std::byte *>(inputData);
  std::copy(inputDataAsBytes, inputDataAsBytes + inputDataSize, data.begin());
  return data;
}

} // namespace ffmpeg
