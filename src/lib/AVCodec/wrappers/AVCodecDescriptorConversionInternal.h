/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/InternalTypes.h>

namespace libffmpeg::internal::avcodec
{

struct AVProfile_57
{
  int         profile;
  const char *name;
};

using AVProfile_58 = AVProfile_57;
using AVProfile_59 = AVProfile_57;
using AVProfile_60 = AVProfile_57;

struct AVCodecDescriptor_56
{
  AVCodecID          id;
  AVMediaType        type;
  const char        *name;
  const char        *long_name;
  int                props;
  const char *const *mime_types;
};

struct AVCodecDescriptor_57
{
  AVCodecID                  id;
  AVMediaType                type;
  const char                *name;
  const char                *long_name;
  int                        props;
  const char *const         *mime_types;
  const struct AVProfile_57 *profiles;
};

using AVCodecDescriptor_58 = AVCodecDescriptor_57;
using AVCodecDescriptor_59 = AVCodecDescriptor_57;
using AVCodecDescriptor_60 = AVCodecDescriptor_57;
using AVCodecDescriptor_61 = AVCodecDescriptor_57;
using AVCodecDescriptor_62 = AVCodecDescriptor_57;

} // namespace libffmpeg::internal::avcodec
