/* LibFFmpeg++
 * Copyright (C) 2023 Christian Feldmann [christian.feldmann@gmx.de]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "AVInputFormatWrapper.h"

namespace LibFFmpeg
{

namespace
{

typedef struct AVInputFormat_56_57_58_59_60
{
  const char *                    name;
  const char *                    long_name;
  int                             flags;
  const char *                    extensions;
  const struct AVCodecTag *const *codec_tag;
  const AVClass *                 priv_class;
  const char *                    mime_type;

  // There is more but it is not part of the public ABI
} AVInputFormat_56_57_58_59_60;

} // namespace

AVInputFormatWrapper::AVInputFormatWrapper(AVInputFormat *        avInputFormat,
                                           const LibraryVersions &libraryVersions)
{
  this->avInputFormat   = avInputFormat;
  this->libraryVersions = libraryVersions;
  this->update();
}

void AVInputFormatWrapper::update()
{
  if (this->avInputFormat == nullptr)
    return;

  if (this->libraryVersions.avformat.major == 56 || //
      this->libraryVersions.avformat.major == 57 || //
      this->libraryVersions.avformat.major == 58 || //
      this->libraryVersions.avformat.major == 59 || //
      this->libraryVersions.avformat.major == 60)
  {
    auto p           = reinterpret_cast<AVInputFormat_56_57_58_59_60 *>(this->avInputFormat);
    this->name       = std::string(p->name);
    this->long_name  = std::string(p->long_name);
    this->flags      = p->flags;
    this->extensions = std::string(p->extensions);
    this->mime_type  = std::string(p->mime_type);
  }
}

} // namespace LibFFmpeg
