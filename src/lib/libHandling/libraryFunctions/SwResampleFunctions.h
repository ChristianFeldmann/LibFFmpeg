/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/FFMpegLibrariesTypes.h>
#include <libHandling/SharedLibraryLoader.h>

namespace ffmpeg::functions
{

struct SwResampleFunctions
{
  std::function<unsigned()> swresample_version;
};

std::optional<SwResampleFunctions> tryBindSwResampleFunctionsFromLibrary(SharedLibraryLoader &lib,
                                                                         Log &                log);

} // namespace ffmpeg::functions
