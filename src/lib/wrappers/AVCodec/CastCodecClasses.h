/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#define CAST_AVCODEC_GET_MEMBER(libraryVersions, classPrefix, castFrom, variableToAssign, member)  \
  {                                                                                                \
    if (libraryVersions.avcodec.major == 56)                                                       \
    {                                                                                              \
      const auto p     = reinterpret_cast<classPrefix##_56 *>(castFrom);                           \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    if (libraryVersions.avcodec.major == 57)                                                       \
    {                                                                                              \
      const auto p     = reinterpret_cast<classPrefix##_57 *>(castFrom);                           \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    if (libraryVersions.avcodec.major == 58)                                                       \
    {                                                                                              \
      const auto p     = reinterpret_cast<classPrefix##_58 *>(castFrom);                           \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    if (libraryVersions.avcodec.major == 59)                                                       \
    {                                                                                              \
      const auto p     = reinterpret_cast<classPrefix##_59 *>(castFrom);                           \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    if (libraryVersions.avcodec.major == 60)                                                       \
    {                                                                                              \
      const auto p     = reinterpret_cast<classPrefix##_60 *>(castFrom);                           \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    else                                                                                           \
      throw std::runtime_error("Invalid library version");                                         \
  }
