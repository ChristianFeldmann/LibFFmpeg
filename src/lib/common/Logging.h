/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <functional>
#include <string>

namespace libffmpeg
{

enum class LogLevel
{
  Debug,
  Info,
  Warning,
  Error
};

struct LogEntry
{
  LogLevel    logLevel{};
  std::string message{};

  bool operator==(const LogEntry &other) const
  {
    return this->logLevel == other.logLevel && this->message == other.message;
  }
};

using LoggingFunction = std::function<void(const LogLevel, const std::string &)>;

} // namespace libffmpeg
