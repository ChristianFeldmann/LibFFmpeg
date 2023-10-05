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

#include "AVFrameSideDataWrapper.h"
#include <stdexcept>

namespace LibFFmpeg
{

namespace
{

typedef struct AVFrameSideData_54_55_56
{
  enum AVFrameSideDataType type;
  uint8_t *                data;
  int                      size;
  AVDictionary *           metadata;
  AVBufferRef *            buf;
} AVFrameSideData_54_55_56;

typedef struct AVFrameSideData_57_58
{
  enum AVFrameSideDataType type;
  uint8_t *                data;
  size_t                   size;
  AVDictionary *           metadata;
  AVBufferRef *            buf;
} AVFrameSideData_57_58;

} // namespace

AVFrameSideDataWrapper::AVFrameSideDataWrapper(AVFrameSideData *      sideData,
                                               const LibraryVersions &libraryVersions)
    : sideData(sideData), libraryVersions(libraryVersions)
{
}

std::vector<MotionVector> AVFrameSideDataWrapper::getMotionVectors() const
{
  if (this->sideData == nullptr)
    return {};

  if (this->libraryVersions.avutil.major == 54 || //
      this->libraryVersions.avutil.major == 55 || //
      this->libraryVersions.avutil.major == 56)
  {
    auto p = reinterpret_cast<AVFrameSideData_54_55_56 *>(sideData);

    if (p->type != AV_FRAME_DATA_MOTION_VECTORS)
      return {};

    return parseMotionData(this->libraryVersions, p->data, p->size);
  }
  else if (this->libraryVersions.avutil.major == 57 || //
           this->libraryVersions.avutil.major == 58)
  {
    auto p = reinterpret_cast<AVFrameSideData_57_58 *>(sideData);

    if (p->type != AV_FRAME_DATA_MOTION_VECTORS)
      return {};

    return parseMotionData(this->libraryVersions, p->data, p->size);
  }
  else
    throw std::runtime_error("Invalid library version");
}

} // namespace LibFFmpeg
