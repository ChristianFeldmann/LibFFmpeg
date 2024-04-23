/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "StaticCallbacks.h"

#include <array>
#include <cstdarg>

namespace ffmpeg
{

/* Here we store the callback functions per loaded ffmpeg library. If multiple ffmpeg libraries
 * are loaded, we need multiple static functions to map each libraries callback function to
 * a dedicated function. However, we can not create an unlimited amount of dedicated functions,
 * so we have a fixed number here. If you open more ffmpeg libraries at once, not all callbacks
 * will work correctly.
 */
constexpr auto                                           MAX_NR_REGISTERED_CALLBACKS = 20;
std::array<LoggingFunction, MAX_NR_REGISTERED_CALLBACKS> staticCallbackVector;

void logToLoggingFunction(
    LoggingFunction &loggingFunction, void *ptr, int level, const char *fmt, va_list vargs)
{
  if (!loggingFunction)
    return;

  std::string message;
  va_list     vargs_copy;
  va_copy(vargs_copy, vargs);
  const auto len = vsnprintf(0, 0, fmt, vargs_copy);
  message.resize(len);
  vsnprintf(&message[0], len + 1, fmt, vargs);

  auto logLevel = LogLevel::Debug;
  if (level <= 16)
    logLevel = LogLevel::Error;
  if (level <= 32)
    logLevel = LogLevel::Info;

  loggingFunction(logLevel, message);
}

template <std::size_t nr>
void staticLogFunction(void *ptr, int level, const char *fmt, va_list vargs)
{
  logToLoggingFunction(staticCallbackVector[nr], ptr, level, fmt, vargs);
}

[[nodiscard]] std::optional<int> setStaticLoggingCallback(AvUtilFunctions &avutilFunctions,
                                                          LoggingFunction  loggingFunction)
{
  int callbackCounter = 0;
  while (callbackCounter < MAX_NR_REGISTERED_CALLBACKS)
  {
    if (!staticCallbackVector.at(callbackCounter))
    {
      staticCallbackVector[callbackCounter] = loggingFunction;
      if (callbackCounter == 0)
        avutilFunctions.av_log_set_callback(staticLogFunction<0>);
      else if (callbackCounter == 1)
        avutilFunctions.av_log_set_callback(staticLogFunction<1>);
      else if (callbackCounter == 2)
        avutilFunctions.av_log_set_callback(staticLogFunction<2>);
      else if (callbackCounter == 3)
        avutilFunctions.av_log_set_callback(staticLogFunction<3>);
      else if (callbackCounter == 4)
        avutilFunctions.av_log_set_callback(staticLogFunction<4>);
      else if (callbackCounter == 5)
        avutilFunctions.av_log_set_callback(staticLogFunction<5>);
      else if (callbackCounter == 6)
        avutilFunctions.av_log_set_callback(staticLogFunction<6>);
      else if (callbackCounter == 7)
        avutilFunctions.av_log_set_callback(staticLogFunction<7>);
      else if (callbackCounter == 8)
        avutilFunctions.av_log_set_callback(staticLogFunction<8>);
      else if (callbackCounter == 9)
        avutilFunctions.av_log_set_callback(staticLogFunction<9>);
      else if (callbackCounter == 10)
        avutilFunctions.av_log_set_callback(staticLogFunction<10>);
      else if (callbackCounter == 11)
        avutilFunctions.av_log_set_callback(staticLogFunction<11>);
      else if (callbackCounter == 12)
        avutilFunctions.av_log_set_callback(staticLogFunction<12>);
      else if (callbackCounter == 13)
        avutilFunctions.av_log_set_callback(staticLogFunction<13>);
      else if (callbackCounter == 14)
        avutilFunctions.av_log_set_callback(staticLogFunction<14>);
      else if (callbackCounter == 15)
        avutilFunctions.av_log_set_callback(staticLogFunction<15>);
      else if (callbackCounter == 16)
        avutilFunctions.av_log_set_callback(staticLogFunction<16>);
      else if (callbackCounter == 17)
        avutilFunctions.av_log_set_callback(staticLogFunction<17>);
      else if (callbackCounter == 18)
        avutilFunctions.av_log_set_callback(staticLogFunction<18>);
      else if (callbackCounter == 19)
        avutilFunctions.av_log_set_callback(staticLogFunction<19>);
      return callbackCounter;
    }
    ++callbackCounter;
  }

  loggingFunction(LogLevel::Error,
                  "Error setting av logging callback. The limit of static callbacks was reached.");
  return {};
}

void unsetStaticLoggingCallback(AvUtilFunctions &avutilFunctions, int loggingFunctionIndex)
{
  if (staticCallbackVector.at(loggingFunctionIndex))
    staticCallbackVector[loggingFunctionIndex] = {};

  avutilFunctions.av_log_set_callback(
      avutilFunctions.av_log_default_callback.target<void(void *, int, const char *, va_list)>());
}

} // namespace ffmpeg
