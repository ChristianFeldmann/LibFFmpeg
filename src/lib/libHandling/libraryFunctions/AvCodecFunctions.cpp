/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AvCodecFunctions.h"

#include "Functions.h"
#include <common/Formatting.h>

namespace ffmpeg::internal::functions
{

std::optional<AvCodecFunctions> tryBindAVCodecFunctionsFromLibrary(const SharedLibraryLoader &lib,
                                                                   const Version avCodecVersion,
                                                                   const LoggingFunction &log)
{
  if (!lib)
  {
    log(LogLevel::Error, "Binding of avCodec functions failed. Library is not loaded.");
    return {};
  }

  AvCodecFunctions         functions;
  std::vector<std::string> missingFunctions;

  lib.tryResolveFunction(functions.avcodec_find_decoder, "avcodec_find_decoder");
  lib.tryResolveFunction(functions.avcodec_alloc_context3, "avcodec_alloc_context3");
  lib.tryResolveFunction(functions.avcodec_open2, "avcodec_open2");
  lib.tryResolveFunction(functions.avcodec_free_context, "avcodec_free_context");
  lib.tryResolveFunction(functions.av_new_packet, "av_new_packet");
  lib.tryResolveFunction(functions.avcodec_flush_buffers, "avcodec_flush_buffers");
  lib.tryResolveFunction(functions.avcodec_version, "avcodec_version");
  lib.tryResolveFunction(functions.avcodec_get_name, "avcodec_get_name");
  lib.tryResolveFunction(functions.avcodec_descriptor_get, "avcodec_descriptor_get");

  checkForMissingFunctionAndLog(
      functions.avcodec_find_decoder, "avcodec_find_decoder", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avcodec_alloc_context3, "avcodec_alloc_context3", missingFunctions, log);
  checkForMissingFunctionAndLog(functions.avcodec_open2, "avcodec_open2", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avcodec_free_context, "avcodec_free_context", missingFunctions, log);
  checkForMissingFunctionAndLog(functions.av_new_packet, "av_new_packet", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avcodec_flush_buffers, "avcodec_flush_buffers", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avcodec_version, "avcodec_version", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avcodec_get_name, "avcodec_get_name", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avcodec_descriptor_get, "avcodec_descriptor_get", missingFunctions, log);

  if (avCodecVersion.major >= 57)
  {
    log(LogLevel::Debug,
        "AVCodec version " + std::to_string(avCodecVersion.major) +
            " is >= 57. Looking for new decoding API.");

    lib.tryResolveFunction(functions.avcodec_parameters_alloc, "avcodec_parameters_alloc");
    lib.tryResolveFunction(functions.av_packet_alloc, "av_packet_alloc");
    lib.tryResolveFunction(functions.av_packet_free, "av_packet_free");
    lib.tryResolveFunction(functions.avcodec_send_packet, "avcodec_send_packet");
    lib.tryResolveFunction(functions.avcodec_receive_frame, "avcodec_receive_frame");
    lib.tryResolveFunction(functions.avcodec_parameters_to_context,
                           "avcodec_parameters_to_context");

    checkForMissingFunctionAndLog(
        functions.avcodec_parameters_alloc, "avcodec_parameters_alloc", missingFunctions, log);
    checkForMissingFunctionAndLog(
        functions.av_packet_alloc, "av_packet_alloc", missingFunctions, log);
    checkForMissingFunctionAndLog(
        functions.av_packet_free, "av_packet_free", missingFunctions, log);
    checkForMissingFunctionAndLog(
        functions.avcodec_send_packet, "avcodec_send_packet", missingFunctions, log);
    checkForMissingFunctionAndLog(
        functions.avcodec_receive_frame, "avcodec_receive_frame", missingFunctions, log);
    checkForMissingFunctionAndLog(functions.avcodec_parameters_to_context,
                                  "avcodec_parameters_to_context",
                                  missingFunctions,
                                  log);
    functions.newParametersAPIAvailable = true;
  }
  else
  {
    log(LogLevel::Debug,
        "AVCodec version " + std::to_string(avCodecVersion.major) +
            " is < 57. Looking for the old decoding API.");

    lib.tryResolveFunction(functions.av_free_packet, "av_free_packet");
    lib.tryResolveFunction(functions.avcodec_decode_video2, "avcodec_decode_video2");
    lib.tryResolveFunction(functions.av_init_packet, "av_init_packet");
    lib.tryResolveFunction(functions.av_copy_packet, "av_copy_packet");

    checkForMissingFunctionAndLog(
        functions.av_free_packet, "av_free_packet", missingFunctions, log);
    checkForMissingFunctionAndLog(
        functions.avcodec_decode_video2, "avcodec_decode_video2", missingFunctions, log);
    checkForMissingFunctionAndLog(
        functions.av_init_packet, "av_init_packet", missingFunctions, log);
    checkForMissingFunctionAndLog(
        functions.av_copy_packet, "av_copy_packet", missingFunctions, log);
  }

  if (!missingFunctions.empty())
  {
    log(LogLevel::Debug,
        "Binding avCodec functions failed. Missing functions: " + to_string(missingFunctions));
    return {};
  }

  log(LogLevel::Debug, "Binding of avCodec functions successful.");
  return functions;
}

} // namespace ffmpeg::internal::functions
