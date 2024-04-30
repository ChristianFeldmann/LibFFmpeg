/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/InternalTypes.h>

namespace libffmpeg::internal::avcodec
{

struct AVCodec_56
{
  const char *          name;
  const char *          long_name;
  AVMediaType           type;
  AVCodecID             id;
  int                   capabilities;
  const AVRational *    supported_framerates;
  const AVPixelFormat * pix_fmts;
  const int *           supported_samplerates;
  const AVSampleFormat *sample_fmts;
  const uint64_t *      channel_layouts;
  uint8_t               max_lowres;
  const AVClass *       priv_class;

  // Actually, there is more here, but nothing more of the public API
};

typedef AVCodec_56 AVCodec_57;
typedef AVCodec_56 AVCodec_58;

struct AVCodec_59
{
  const char *          name;
  const char *          long_name;
  AVMediaType           type;
  AVCodecID             id;
  int                   capabilities;
  uint8_t               max_lowres;
  const AVRational *    supported_framerates;
  const AVPixelFormat * pix_fmts;
  const int *           supported_samplerates;
  const AVSampleFormat *sample_fmts;
  const uint64_t *      channel_layouts;
  const AVClass *       priv_class;

  // Actually, there is more here, but nothing more of the public API
};

typedef AVCodec_59 AVCodec_60;

struct AVCodec_61
{
  const char *          name;
  const char *          long_name;
  AVMediaType           type;
  AVCodecID             id;
  int                   capabilities;
  uint8_t               max_lowres;
  const AVRational *    supported_framerates;
  const AVPixelFormat * pix_fmts;
  const int *           supported_samplerates;
  const AVSampleFormat *sample_fmts;
  const AVClass *       priv_class;
  const AVProfile *     profiles;
  const char *          wrapper_name;
  const uint64_t *      channel_layouts;
};

} // namespace libffmpeg::internal::avcodec
