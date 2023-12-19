/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/InternalTypes.h>

namespace ffmpeg::internal::avcodec
{

struct AVProfile_56
{
  int         profile;
  const char *name;
};

typedef AVProfile_56 AVProfile_57;
typedef AVProfile_56 AVProfile_58;
typedef AVProfile_56 AVProfile_59;
typedef AVProfile_56 AVProfile_60;

struct AVCodecDescriptor_56
{
  AVCodecID                  id;
  AVMediaType                type;
  const char                *name;
  const char                *long_name;
  int                        props;
  const char *const         *mime_types;
  const struct AVProfile_56 *profiles;
};

typedef AVCodecDescriptor_56 AVCodecDescriptor_57;
typedef AVCodecDescriptor_56 AVCodecDescriptor_58;
typedef AVCodecDescriptor_56 AVCodecDescriptor_59;
typedef AVCodecDescriptor_56 AVCodecDescriptor_60;

} // namespace ffmpeg::internal::avcodec
