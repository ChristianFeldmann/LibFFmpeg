/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#define CAST_AVCODEC_GET_MEMBER(classPrefix, castFrom, variableToAssign, member)                   \
  {                                                                                                \
    if (this->librariesInterface->getLibrariesVersion().avcodec.major == 56)                       \
    {                                                                                              \
      const auto p     = reinterpret_cast<internal::classPrefix##_56 *>(castFrom);                 \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    else if (this->librariesInterface->getLibrariesVersion().avcodec.major == 57)                  \
    {                                                                                              \
      const auto p     = reinterpret_cast<internal::classPrefix##_57 *>(castFrom);                 \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    else if (this->librariesInterface->getLibrariesVersion().avcodec.major == 58)                  \
    {                                                                                              \
      const auto p     = reinterpret_cast<internal::classPrefix##_58 *>(castFrom);                 \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    else if (this->librariesInterface->getLibrariesVersion().avcodec.major == 59)                  \
    {                                                                                              \
      const auto p     = reinterpret_cast<internal::classPrefix##_59 *>(castFrom);                 \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    else if (this->librariesInterface->getLibrariesVersion().avcodec.major == 60)                  \
    {                                                                                              \
      const auto p     = reinterpret_cast<internal::classPrefix##_60 *>(castFrom);                 \
      variableToAssign = p->member;                                                                \
    }                                                                                              \
    else                                                                                           \
      throw std::runtime_error("Invalid library version");                                         \
  }

#define CAST_AVCODEC_SET_MEMBER(classPrefix, castFrom, member, variableToSet)                      \
  {                                                                                                \
    if (castFrom == nullptr)                                                                       \
      throw std::runtime_error("Cast from nullptr");                                               \
    if (this->librariesInterface->getLibrariesVersion().avformat.major == 56)                      \
    {                                                                                              \
      const auto p = reinterpret_cast<internal::classPrefix##_56 *>(castFrom);                     \
      p->member    = variableToSet;                                                                \
    }                                                                                              \
    else if (this->librariesInterface->getLibrariesVersion().avformat.major == 57)                 \
    {                                                                                              \
      const auto p = reinterpret_cast<internal::classPrefix##_57 *>(castFrom);                     \
      p->member    = variableToSet;                                                                \
    }                                                                                              \
    else if (this->librariesInterface->getLibrariesVersion().avformat.major == 58)                 \
    {                                                                                              \
      const auto p = reinterpret_cast<internal::classPrefix##_58 *>(castFrom);                     \
      p->member    = variableToSet;                                                                \
    }                                                                                              \
    else if (this->librariesInterface->getLibrariesVersion().avformat.major == 59)                 \
    {                                                                                              \
      const auto p = reinterpret_cast<internal::classPrefix##_59 *>(castFrom);                     \
      p->member    = variableToSet;                                                                \
    }                                                                                              \
    else if (this->librariesInterface->getLibrariesVersion().avformat.major == 60)                 \
    {                                                                                              \
      const auto p = reinterpret_cast<internal::classPrefix##_60 *>(castFrom);                     \
      p->member    = variableToSet;                                                                \
    }                                                                                              \
    else                                                                                           \
      throw std::runtime_error("Invalid library version");                                         \
  }
