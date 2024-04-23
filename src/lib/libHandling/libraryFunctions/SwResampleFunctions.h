/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/Logging.h>
#include <common/Types.h>
#include <libHandling/SharedLibraryLoader.h>

namespace ffmpeg::internal::functions
{

struct SwResampleFunctions
{
  std::function<unsigned()> swresample_version;
};

std::optional<SwResampleFunctions>
tryBindSwResampleFunctionsFromLibrary(const SharedLibraryLoader &lib, const LoggingFunction &log);

} // namespace ffmpeg::internal::functions
