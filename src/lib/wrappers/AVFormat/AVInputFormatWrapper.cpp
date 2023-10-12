/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVInputFormatWrapper.h"

#include "CastFormatClasses.h"

namespace ffmpeg::avformat
{

namespace
{

struct AVInputFormat_56
{
  const char                     *name;
  const char                     *long_name;
  int                             flags;
  const char                     *extensions;
  const struct AVCodecTag *const *codec_tag;
  const AVClass                  *priv_class;
  const char                     *mime_type;

  // There is more but it is not part of the public ABI
};

typedef AVInputFormat_56 AVInputFormat_57;
typedef AVInputFormat_56 AVInputFormat_58;
typedef AVInputFormat_56 AVInputFormat_59;
typedef AVInputFormat_56 AVInputFormat_60;

} // namespace

AVInputFormatWrapper::AVInputFormatWrapper(
    AVInputFormat *inputFormat, std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
    : inputFormat(inputFormat), librariesInterface(librariesInterface)
{
}

std::string AVInputFormatWrapper::getName() const
{
  std::string name;
  CAST_AVFORMAT_GET_MEMBER(AVInputFormat, this->inputFormat, name, name);
  return name;
}

} // namespace ffmpeg::avformat
