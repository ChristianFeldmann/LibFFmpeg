/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

namespace ffmpeg::internal::avformat
{

constexpr auto AVFMT_NOFILE        = 0x0001;
constexpr auto AVFMT_NEEDNUMBER    = 0x0002;
constexpr auto AVFMT_EXPERIMENTAL  = 0x0004;
constexpr auto AVFMT_SHOW_IDS      = 0x0008;
constexpr auto AVFMT_GLOBALHEADER  = 0x0040;
constexpr auto AVFMT_NOTIMESTAMPS  = 0x0080;
constexpr auto AVFMT_GENERIC_INDEX = 0x0100;
constexpr auto AVFMT_TS_DISCONT    = 0x0200;
constexpr auto AVFMT_VARIABLE_FPS  = 0x0400;
constexpr auto AVFMT_NODIMENSIONS  = 0x0800;
constexpr auto AVFMT_NOSTREAMS     = 0x1000;
constexpr auto AVFMT_NOBINSEARCH   = 0x2000;
constexpr auto AVFMT_NOGENSEARCH   = 0x4000;
constexpr auto AVFMT_NO_BYTE_SEEK  = 0x8000;
constexpr auto AVFMT_ALLOW_FLUSH   = 0x10000;
constexpr auto AVFMT_TS_NONSTRICT  = 0x20000;
constexpr auto AVFMT_TS_NEGATIVE   = 0x40000;
constexpr auto AVFMT_SEEK_TO_PTS   = 0x4000000;

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

} // namespace ffmpeg::internal::avformat
