/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <AVUtil/wrappers/AVFrameWrapper.h>

#include <fstream>

namespace ffmpeg::test
{

class FrameFileWriter
{
public:
  FrameFileWriter() = delete;
  FrameFileWriter(std::string filename);

  void write(const avutil::AVFrameWrapper &frame);

private:
  std::ofstream outfile;
};

} // namespace ffmpeg::test
