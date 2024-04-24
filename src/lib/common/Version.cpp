/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Version.h"

#include <sstream>

namespace libffmpeg
{

Version Version::fromFFmpegVersion(const unsigned ffmpegVersion)
{
  Version v;
  v.major = ((ffmpegVersion & 0xFF0000) >> 16);
  v.minor = ((ffmpegVersion & 0xFF00) >> 8);
  v.micro = (ffmpegVersion & 0xFF);
  return v;
}

bool Version::operator!=(const Version &other) const
{
  if (this->major != other.major)
    return true;
  if (this->minor && other.minor && this->minor.value() != other.minor.value())
    return true;
  if (this->micro && other.micro && this->micro.value() != other.micro.value())
    return true;

  return false;
}

std::string Version::toString() const
{
  std::ostringstream stream;
  stream << "v" << this->major;
  if (this->minor)
  {
    stream << "." << this->minor.value();
    if (this->micro)
      stream << "." << this->micro.value();
  }
  return stream.str();
}

std::ostream &Version::operator<<(std::ostream &stream) const
{
  stream << this->toString();
  return stream;
}

} // namespace libffmpeg
