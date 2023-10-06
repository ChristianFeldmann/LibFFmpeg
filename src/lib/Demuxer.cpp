/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Demuxer.h"

namespace LibFFmpeg
{

Demuxer::Demuxer(std::shared_ptr<FFmpegLibrariesInterface> libraries)
{
  this->libraries = libraries;
}

} // namespace LibFFmpeg
