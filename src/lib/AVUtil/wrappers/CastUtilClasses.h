/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#define CAST_AVUTIL_GET_MEMBER(classPrefix, castFrom, variableToAssign, member)                    \
  {                                                                                                \
    if (this->ffmpegLibraries->getLibrariesVersion().avutil.major == 54)                           \
    {                                                                                              \
      const auto p     = reinterpret_cast<internal::avutil::classPrefix##_54 *>(castFrom);         \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    else if (this->ffmpegLibraries->getLibrariesVersion().avutil.major == 55)                      \
    {                                                                                              \
      const auto p     = reinterpret_cast<internal::avutil::classPrefix##_55 *>(castFrom);         \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    else if (this->ffmpegLibraries->getLibrariesVersion().avutil.major == 56)                      \
    {                                                                                              \
      const auto p     = reinterpret_cast<internal::avutil::classPrefix##_56 *>(castFrom);         \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    else if (this->ffmpegLibraries->getLibrariesVersion().avutil.major == 57)                      \
    {                                                                                              \
      const auto p     = reinterpret_cast<internal::avutil::classPrefix##_57 *>(castFrom);         \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    else if (this->ffmpegLibraries->getLibrariesVersion().avutil.major == 58)                      \
    {                                                                                              \
      const auto p     = reinterpret_cast<internal::avutil::classPrefix##_58 *>(castFrom);         \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    else if (this->ffmpegLibraries->getLibrariesVersion().avutil.major == 59)                      \
    {                                                                                              \
      const auto p     = reinterpret_cast<internal::avutil::classPrefix##_59 *>(castFrom);         \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    else if (this->ffmpegLibraries->getLibrariesVersion().avutil.major == 60)                      \
    {                                                                                              \
      const auto p     = reinterpret_cast<internal::avutil::classPrefix##_60 *>(castFrom);         \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    else                                                                                           \
      throw std::runtime_error("Invalid library version");                                         \
  }
