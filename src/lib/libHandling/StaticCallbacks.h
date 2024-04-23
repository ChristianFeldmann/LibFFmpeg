/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <common/Logging.h>
#include <libHandling/libraryFunctions/AvUtilFunctions.h>

#include <optional>

namespace ffmpeg
{

using AvUtilFunctions = ffmpeg::internal::functions::AvUtilFunctions;

[[nodiscard]] std::optional<int> setStaticLoggingCallback(AvUtilFunctions &avutilFunctions,
                                                          LoggingFunction  loggingFunction);

void unsetStaticLoggingCallback(AvUtilFunctions &avutilFunctions, const int loggingFunctionIndex);

} // namespace ffmpeg
