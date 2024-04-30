/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "DebugFunctions.h"

namespace libffmpeg::test
{

FrameFileWriter::FrameFileWriter(std::string filename)
{
  this->outfile.open(filename, std::ios::binary | std::ios::out);
}

void FrameFileWriter::write(const avutil::AVFrameWrapper &frame)
{
  const auto pixelFormat = frame.getPixelFormatDescriptor();

  for (int component = 0; component < pixelFormat.numberOfComponents; ++component)
  {
    const auto data = frame.getData(component);
    this->outfile.write(reinterpret_cast<const char *>(data.data()), data.size());
  }
}

} // namespace libffmpeg::test
