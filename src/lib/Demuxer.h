/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <libHandling/FFmpegLibrariesInterface.h>

namespace LibFFmpeg
{

class Demuxer
{
public:
  Demuxer(std::shared_ptr<FFmpegLibrariesInterface> libraries);

private:
  std::shared_ptr<FFmpegLibrariesInterface> libraries;
};

} // namespace LibFFmpeg
