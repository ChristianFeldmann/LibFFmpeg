/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AvCodecFunctions.h"

#include "Functions.h"

namespace ffmpeg::functions
{

std::optional<AvCodecFunctions> tryBindAVCodecFunctionsFromLibrary(SharedLibraryLoader &lib,
                                                                   Log                 &log)
{
  if (!lib)
  {
    log.push_back("Binding of avCodec functions failed. Library is not loaded.");
    return {};
  }

  AvCodecFunctions functions;

  lib.tryResolveFunction(functions.avcodec_find_decoder, "avcodec_find_decoder");
  lib.tryResolveFunction(functions.avcodec_alloc_context3, "avcodec_alloc_context3");
  lib.tryResolveFunction(functions.avcodec_open2, "avcodec_open2");
  lib.tryResolveFunction(functions.avcodec_free_context, "avcodec_free_context");
  lib.tryResolveFunction(functions.av_packet_alloc, "av_packet_alloc");
  lib.tryResolveFunction(functions.av_new_packet, "av_new_packet");
  lib.tryResolveFunction(functions.av_packet_unref, "av_packet_unref");
  lib.tryResolveFunction(functions.avcodec_flush_buffers, "avcodec_flush_buffers");
  lib.tryResolveFunction(functions.avcodec_version, "avcodec_version");
  lib.tryResolveFunction(functions.avcodec_get_name, "avcodec_get_name");
  lib.tryResolveFunction(functions.avcodec_parameters_alloc, "avcodec_parameters_alloc");
  lib.tryResolveFunction(functions.avcodec_descriptor_get, "avcodec_descriptor_get");
  lib.tryResolveFunction(functions.avcodec_send_packet, "avcodec_send_packet");
  lib.tryResolveFunction(functions.avcodec_receive_frame, "avcodec_receive_frame");
  lib.tryResolveFunction(functions.avcodec_parameters_to_context, "avcodec_parameters_to_context");
  lib.tryResolveFunction(functions.avcodec_decode_video2, "avcodec_decode_video2");

  std::vector<std::string> missingFunctions;

  checkForMissingFunctionAndLog(
      functions.avcodec_find_decoder, "avcodec_find_decoder", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avcodec_alloc_context3, "avcodec_alloc_context3", missingFunctions, log);
  checkForMissingFunctionAndLog(functions.avcodec_open2, "avcodec_open2", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avcodec_free_context, "avcodec_free_context", missingFunctions, log);
  checkForMissingFunctionAndLog(functions.av_new_packet, "av_new_packet", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.av_packet_alloc, "av_packet_alloc", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.av_packet_unref, "av_packet_unref", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avcodec_flush_buffers, "avcodec_flush_buffers", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avcodec_version, "avcodec_version", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avcodec_get_name, "avcodec_get_name", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avcodec_parameters_alloc, "avcodec_parameters_alloc", missingFunctions, log);
  checkForMissingFunctionAndLog(
      functions.avcodec_descriptor_get, "avcodec_descriptor_get", missingFunctions, log);

  std::vector<std::string> missingFunctionsNewApi;

  checkForMissingFunctionAndLog(
      functions.avcodec_send_packet, "avcodec_send_packet", missingFunctionsNewApi, log);
  checkForMissingFunctionAndLog(
      functions.avcodec_receive_frame, "avcodec_receive_frame", missingFunctionsNewApi, log);
  checkForMissingFunctionAndLog(functions.avcodec_parameters_to_context,
                                "avcodec_parameters_to_context",
                                missingFunctionsNewApi,
                                log);

  if (missingFunctionsNewApi.empty())
  {
    log.push_back("New Decoding API found. Skipping check for old API function.");
    functions.newParametersAPIAvailable = true;
  }
  else
  {
    log.push_back("New Decoding API not found. Missing functions " +
                  to_string(missingFunctionsNewApi) + ". Checking old decoding API.");
    checkForMissingFunctionAndLog(
        functions.avcodec_decode_video2, "avcodec_decode_video2", missingFunctions, log);
  }

  if (!missingFunctions.empty())
  {
    log.push_back("Binding avCodec functions failed. Missing functions: " +
                  to_string(missingFunctions));
    return {};
  }

  log.push_back("Binding of avCodec functions successful.");
  return functions;
}

} // namespace ffmpeg::functions
