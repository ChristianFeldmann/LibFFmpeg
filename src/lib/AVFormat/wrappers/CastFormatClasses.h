/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#define CAST_AVFORMAT_GET_MEMBER(classPrefix, castFrom, variableToGetInto, member)                 \
  {                                                                                                \
    if (castFrom == nullptr)                                                                       \
      throw std::runtime_error("Cast from nullptr");                                               \
    if (this->ffmpegLibraries->getLibrariesVersion().avformat.major == 56)                         \
    {                                                                                              \
      const auto p      = reinterpret_cast<classPrefix##_56 *>(castFrom);                          \
      variableToGetInto = p->member;                                                               \
    }                                                                                              \
    else if (this->ffmpegLibraries->getLibrariesVersion().avformat.major == 57)                    \
    {                                                                                              \
      const auto p      = reinterpret_cast<classPrefix##_57 *>(castFrom);                          \
      variableToGetInto = p->member;                                                               \
    }                                                                                              \
    else if (this->ffmpegLibraries->getLibrariesVersion().avformat.major == 58)                    \
    {                                                                                              \
      const auto p      = reinterpret_cast<classPrefix##_58 *>(castFrom);                          \
      variableToGetInto = p->member;                                                               \
    }                                                                                              \
    else if (this->ffmpegLibraries->getLibrariesVersion().avformat.major == 59)                    \
    {                                                                                              \
      const auto p      = reinterpret_cast<classPrefix##_59 *>(castFrom);                          \
      variableToGetInto = p->member;                                                               \
    }                                                                                              \
    else if (this->ffmpegLibraries->getLibrariesVersion().avformat.major == 60)                    \
    {                                                                                              \
      const auto p      = reinterpret_cast<classPrefix##_60 *>(castFrom);                          \
      variableToGetInto = p->member;                                                               \
    }                                                                                              \
    else if (this->ffmpegLibraries->getLibrariesVersion().avformat.major == 61)                    \
    {                                                                                              \
      const auto p      = reinterpret_cast<classPrefix##_61 *>(castFrom);                          \
      variableToGetInto = p->member;                                                               \
    }                                                                                              \
    else                                                                                           \
      throw std::runtime_error("Invalid library version");                                         \
  }

#define CAST_AVFORMAT_SET_MEMBER(classPrefix, castFrom, member, variableToSet)                     \
  {                                                                                                \
    if (castFrom == nullptr)                                                                       \
      throw std::runtime_error("Cast from nullptr");                                               \
    if (this->ffmpegLibraries->getLibrariesVersion().avformat.major == 56)                         \
    {                                                                                              \
      const auto p = reinterpret_cast<classPrefix##_56 *>(castFrom);                               \
      p->member    = variableToSet;                                                                \
    }                                                                                              \
    else if (this->ffmpegLibraries->getLibrariesVersion().avformat.major == 57)                    \
    {                                                                                              \
      const auto p = reinterpret_cast<classPrefix##_57 *>(castFrom);                               \
      p->member    = variableToSet;                                                                \
    }                                                                                              \
    else if (this->ffmpegLibraries->getLibrariesVersion().avformat.major == 58)                    \
    {                                                                                              \
      const auto p = reinterpret_cast<classPrefix##_58 *>(castFrom);                               \
      p->member    = variableToSet;                                                                \
    }                                                                                              \
    else if (this->ffmpegLibraries->getLibrariesVersion().avformat.major == 59)                    \
    {                                                                                              \
      const auto p = reinterpret_cast<classPrefix##_59 *>(castFrom);                               \
      p->member    = variableToSet;                                                                \
    }                                                                                              \
    else if (this->ffmpegLibraries->getLibrariesVersion().avformat.major == 60)                    \
    {                                                                                              \
      const auto p = reinterpret_cast<classPrefix##_60 *>(castFrom);                               \
      p->member    = variableToSet;                                                                \
    }                                                                                              \
    else if (this->ffmpegLibraries->getLibrariesVersion().avformat.major == 61)                    \
    {                                                                                              \
      const auto p = reinterpret_cast<classPrefix##_61 *>(castFrom);                               \
      p->member    = variableToSet;                                                                \
    }                                                                                              \
    else                                                                                           \
      throw std::runtime_error("Invalid library version");                                         \
  }
