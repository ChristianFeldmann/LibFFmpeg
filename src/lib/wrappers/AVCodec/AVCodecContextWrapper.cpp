/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVCodecContextWrapper.h"

#include <common/InternalTypes.h>
#include <wrappers/AVUtil/AVDictionaryWrapper.h>
#include <wrappers/Functions.h>

#include "CastCodecClasses.h"

namespace ffmpeg::avcodec
{

using ffmpeg::internal::AVClass;
using ffmpeg::internal::AVCodecContext;
using ffmpeg::internal::AVCodecID;
using ffmpeg::internal::AVColorSpace;
using ffmpeg::internal::AVDictionary;
using ffmpeg::internal::AVFrame;
using ffmpeg::internal::AVMediaType;
using ffmpeg::internal::AVPixelFormat;
using ffmpeg::internal::AVRational;

namespace internal
{

struct AVCodecContext_56
{
  const AVClass *av_class;
  int            log_level_offset;

  AVMediaType             codec_type;
  const struct AVCodec   *codec;
  char                    codec_name[32];
  AVCodecID               codec_id;
  unsigned int            codec_tag;
  unsigned int            stream_codec_tag;
  void                   *priv_data;
  struct AVCodecInternal *internal;
  void                   *opaque;
  int                     bit_rate;
  int                     bit_rate_tolerance;
  int                     global_quality;
  int                     compression_level;
  int                     flags;
  int                     flags2;
  uint8_t                *extradata;
  int                     extradata_size;
  AVRational              time_base;
  int                     ticks_per_frame;
  int                     delay;
  int                     width, height;
  int                     coded_width, coded_height;
  int                     gop_size;
  AVPixelFormat           pix_fmt;
  int                     me_method;
  void (*draw_horiz_band)(struct AVCodecContext *s,
                          const AVFrame         *src,
                          int                    offset[AV_NUM_DATA_POINTERS],
                          int                    y,
                          int                    type,
                          int                    height);
  AVPixelFormat (*get_format)(struct AVCodecContext *s, const AVPixelFormat *fmt);
  int                           max_b_frames;
  float                         b_quant_factor;
  int                           rc_strategy;
  int                           b_frame_strategy;
  float                         b_quant_offset;
  int                           has_b_frames;
  int                           mpeg_quant;
  float                         i_quant_factor;
  float                         i_quant_offset;
  float                         lumi_masking;
  float                         temporal_cplx_masking;
  float                         spatial_cplx_masking;
  float                         p_masking;
  float                         dark_masking;
  int                           slice_count;
  int                           prediction_method;
  int                          *slice_offset;
  AVRational                    sample_aspect_ratio;
  int                           me_cmp;
  int                           me_sub_cmp;
  int                           mb_cmp;
  int                           ildct_cmp;
  int                           dia_size;
  int                           last_predictor_count;
  int                           pre_me;
  int                           me_pre_cmp;
  int                           pre_dia_size;
  int                           me_subpel_quality;
  int                           dtg_active_format;
  int                           me_range;
  int                           intra_quant_bias;
  int                           inter_quant_bias;
  int                           slice_flags;
  int                           xvmc_acceleration;
  int                           mb_decision;
  uint16_t                     *intra_matrix;
  uint16_t                     *inter_matrix;
  int                           scenechange_threshold;
  int                           noise_reduction;
  int                           me_threshold;
  int                           mb_threshold;
  int                           intra_dc_precision;
  int                           skip_top;
  int                           skip_bottom;
  float                         border_masking;
  int                           mb_lmin;
  int                           mb_lmax;
  int                           me_penalty_compensation;
  int                           bidir_refine;
  int                           brd_scale;
  int                           keyint_min;
  int                           refs;
  int                           chromaoffset;
  int                           scenechange_factor;
  int                           mv0_threshold;
  int                           b_sensitivity;
  AVColorPrimaries              color_primaries;
  AVColorTransferCharacteristic color_trc;
  AVColorSpace                  colorspace;
  AVColorRange                  color_range;
  AVChromaLocation              chroma_sample_location;

  // Actually, there is more here, but the variables above are the only we need.
};

struct AVCodecContext_57
{
  const AVClass          *av_class;
  int                     log_level_offset;
  AVMediaType             codec_type;
  const struct AVCodec   *codec;
  char                    codec_name[32];
  AVCodecID               codec_id;
  unsigned int            codec_tag;
  unsigned int            stream_codec_tag;
  void                   *priv_data;
  struct AVCodecInternal *internal;
  void                   *opaque;
  int64_t                 bit_rate;
  int                     bit_rate_tolerance;
  int                     global_quality;
  int                     compression_level;
  int                     flags;
  int                     flags2;
  uint8_t                *extradata;
  int                     extradata_size;
  AVRational              time_base;
  int                     ticks_per_frame;
  int                     delay;
  int                     width, height;
  int                     coded_width, coded_height;
  int                     gop_size;
  AVPixelFormat           pix_fmt;
  int                     me_method;
  void (*draw_horiz_band)(struct AVCodecContext *s,
                          const AVFrame         *src,
                          int                    offset[AV_NUM_DATA_POINTERS],
                          int                    y,
                          int                    type,
                          int                    height);
  AVPixelFormat (*get_format)(struct AVCodecContext *s, const AVPixelFormat *fmt);
  int                           max_b_frames;
  float                         b_quant_factor;
  int                           rc_strategy;
  int                           b_frame_strategy;
  float                         b_quant_offset;
  int                           has_b_frames;
  int                           mpeg_quant;
  float                         i_quant_factor;
  float                         i_quant_offset;
  float                         lumi_masking;
  float                         temporal_cplx_masking;
  float                         spatial_cplx_masking;
  float                         p_masking;
  float                         dark_masking;
  int                           slice_count;
  int                           prediction_method;
  int                          *slice_offset;
  AVRational                    sample_aspect_ratio;
  int                           me_cmp;
  int                           me_sub_cmp;
  int                           mb_cmp;
  int                           ildct_cmp;
  int                           dia_size;
  int                           last_predictor_count;
  int                           pre_me;
  int                           me_pre_cmp;
  int                           pre_dia_size;
  int                           me_subpel_quality;
  int                           dtg_active_format;
  int                           me_range;
  int                           intra_quant_bias;
  int                           inter_quant_bias;
  int                           slice_flags;
  int                           xvmc_acceleration;
  int                           mb_decision;
  uint16_t                     *intra_matrix;
  uint16_t                     *inter_matrix;
  int                           scenechange_threshold;
  int                           noise_reduction;
  int                           me_threshold;
  int                           mb_threshold;
  int                           intra_dc_precision;
  int                           skip_top;
  int                           skip_bottom;
  float                         border_masking;
  int                           mb_lmin;
  int                           mb_lmax;
  int                           me_penalty_compensation;
  int                           bidir_refine;
  int                           brd_scale;
  int                           keyint_min;
  int                           refs;
  int                           chromaoffset;
  int                           scenechange_factor;
  int                           mv0_threshold;
  int                           b_sensitivity;
  AVColorPrimaries              color_primaries;
  AVColorTransferCharacteristic color_trc;
  AVColorSpace                  colorspace;
  AVColorRange                  color_range;
  AVChromaLocation              chroma_sample_location;

  // Actually, there is more here, but the variables above are the only we need.
};

struct AVCodecContext_58
{
  const AVClass          *av_class;
  int                     log_level_offset;
  AVMediaType             codec_type;
  const struct AVCodec   *codec;
  AVCodecID               codec_id;
  unsigned int            codec_tag;
  void                   *priv_data;
  struct AVCodecInternal *internal;
  void                   *opaque;
  int64_t                 bit_rate;
  int                     bit_rate_tolerance;
  int                     global_quality;
  int                     compression_level;
  int                     flags;
  int                     flags2;
  uint8_t                *extradata;
  int                     extradata_size;
  AVRational              time_base;
  int                     ticks_per_frame;
  int                     delay;
  int                     width, height;
  int                     coded_width, coded_height;
  int                     gop_size;
  AVPixelFormat           pix_fmt;
  void (*draw_horiz_band)(struct AVCodecContext *s,
                          const AVFrame         *src,
                          int                    offset[AV_NUM_DATA_POINTERS],
                          int                    y,
                          int                    type,
                          int                    height);
  AVPixelFormat (*get_format)(struct AVCodecContext *s, const AVPixelFormat *fmt);
  int                                max_b_frames;
  float                              b_quant_factor;
  int                                b_frame_strategy;
  float                              b_quant_offset;
  int                                has_b_frames;
  int                                mpeg_quant;
  float                              i_quant_factor;
  float                              i_quant_offset;
  float                              lumi_masking;
  float                              temporal_cplx_masking;
  float                              spatial_cplx_masking;
  float                              p_masking;
  float                              dark_masking;
  int                                slice_count;
  int                                prediction_method;
  int                               *slice_offset;
  AVRational                         sample_aspect_ratio;
  int                                me_cmp;
  int                                me_sub_cmp;
  int                                mb_cmp;
  int                                ildct_cmp;
  int                                dia_size;
  int                                last_predictor_count;
  int                                pre_me;
  int                                me_pre_cmp;
  int                                pre_dia_size;
  int                                me_subpel_quality;
  int                                me_range;
  int                                slice_flags;
  int                                mb_decision;
  uint16_t                          *intra_matrix;
  uint16_t                          *inter_matrix;
  int                                scenechange_threshold;
  int                                noise_reduction;
  int                                intra_dc_precision;
  int                                skip_top;
  int                                skip_bottom;
  int                                mb_lmin;
  int                                mb_lmax;
  int                                me_penalty_compensation;
  int                                bidir_refine;
  int                                brd_scale;
  int                                keyint_min;
  int                                refs;
  int                                chromaoffset;
  int                                mv0_threshold;
  int                                b_sensitivity;
  enum AVColorPrimaries              color_primaries;
  enum AVColorTransferCharacteristic color_trc;
  AVColorSpace                       colorspace;
  enum AVColorRange                  color_range;
  enum AVChromaLocation              chroma_sample_location;
  int                                slices;

  // Actually, there is more here, but the variables above are the only we need.
};

struct AVCodecContext_59
{
  const AVClass          *av_class;
  int                     log_level_offset;
  AVMediaType             codec_type;
  const struct AVCodec   *codec;
  AVCodecID               codec_id;
  unsigned int            codec_tag;
  void                   *priv_data;
  struct AVCodecInternal *internal;
  void                   *opaque;
  int64_t                 bit_rate;
  int                     bit_rate_tolerance;
  int                     global_quality;
  int                     compression_level;
  int                     flags;
  int                     flags2;
  uint8_t                *extradata;
  int                     extradata_size;
  AVRational              time_base;
  int                     ticks_per_frame;
  int                     delay;
  int                     width, height;
  int                     coded_width, coded_height;
  int                     gop_size;
  AVPixelFormat           pix_fmt;
  void (*draw_horiz_band)(struct AVCodecContext *s,
                          const AVFrame         *src,
                          int                    offset[AV_NUM_DATA_POINTERS],
                          int                    y,
                          int                    type,
                          int                    height);
  AVPixelFormat (*get_format)(struct AVCodecContext *s, const AVPixelFormat *fmt);
  int                                max_b_frames;
  float                              b_quant_factor;
  float                              b_quant_offset;
  int                                has_b_frames;
  float                              i_quant_factor;
  float                              i_quant_offset;
  float                              lumi_masking;
  float                              temporal_cplx_masking;
  float                              spatial_cplx_masking;
  float                              p_masking;
  float                              dark_masking;
  int                                slice_count;
  int                               *slice_offset;
  AVRational                         sample_aspect_ratio;
  int                                me_cmp;
  int                                me_sub_cmp;
  int                                mb_cmp;
  int                                ildct_cmp;
  int                                dia_size;
  int                                last_predictor_count;
  int                                me_pre_cmp;
  int                                pre_dia_size;
  int                                me_subpel_quality;
  int                                me_range;
  int                                slice_flags;
  int                                mb_decision;
  uint16_t                          *intra_matrix;
  uint16_t                          *inter_matrix;
  int                                intra_dc_precision;
  int                                skip_top;
  int                                skip_bottom;
  int                                mb_lmin;
  int                                mb_lmax;
  int                                bidir_refine;
  int                                keyint_min;
  int                                refs;
  int                                mv0_threshold;
  enum AVColorPrimaries              color_primaries;
  enum AVColorTransferCharacteristic color_trc;
  AVColorSpace                       colorspace;
  enum AVColorRange                  color_range;
  enum AVChromaLocation              chroma_sample_location;
  int                                slices;

  // Actually, there is more here, but the variables above are the only we need.
};

typedef AVCodecContext_59 AVCodecContext_60;

} // namespace internal

AVCodecContextWrapper::AVCodecContextWrapper(
    AVCodecContext *codecContext, std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
    : codecContext(codecContext), librariesInterface(librariesInterface)
{
}

std::optional<AVCodecContextWrapper> AVCodecContextWrapper::openContextForDecoding(
    const avformat::AVCodecParametersWrapper &codecParameters,
    std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
{
  const auto decoderCodec =
      librariesInterface->avcodec.avcodec_find_decoder(codecParameters.getCodecID());
  if (decoderCodec == nullptr)
    return {};

  auto codecContext = librariesInterface->avcodec.avcodec_alloc_context3(decoderCodec);
  if (codecContext == nullptr)
    return {};

  auto ret = librariesInterface->avcodec.avcodec_parameters_to_context(
      codecContext, codecParameters.getCodecParameters());
  if (ret < 0)
    return {};

  AVDictionary *dictionary = nullptr;
  ret = librariesInterface->avcodec.avcodec_open2(codecContext, decoderCodec, &dictionary);
  if (ret < 0)
    return {};

  return AVCodecContextWrapper(codecContext, librariesInterface);
}

MediaType AVCodecContextWrapper::getCodecType() const
{
  ffmpeg::internal::AVMediaType type;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, type, codec_type);
  return ffmpeg::internal::toMediaType(type);
}

AVCodecID AVCodecContextWrapper::getCodecID() const
{
  AVCodecID id;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, id, codec_id);
  return id;
}

avutil::AVPixFmtDescriptorWrapper AVCodecContextWrapper::getPixelFormat() const
{
  ffmpeg::internal::AVPixelFormat avPixelFormat;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, avPixelFormat, pix_fmt);
  return avutil::AVPixFmtDescriptorWrapper(avPixelFormat, this->librariesInterface);
}

Size AVCodecContextWrapper::getSize() const
{
  int width;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, width, width);

  int height;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, height, height);

  return {width, height};
}

ColorSpace AVCodecContextWrapper::getColorspace() const
{
  ffmpeg::internal::AVColorSpace avColorspace;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, avColorspace, colorspace);
  return ffmpeg::internal::toColorspace(avColorspace);
}

Rational AVCodecContextWrapper::getTimeBase() const
{
  ffmpeg::internal::AVRational timebase;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, timebase, time_base);
  return Rational({timebase.num, timebase.den});
}

ByteVector AVCodecContextWrapper::getExtradata() const
{
  uint8_t *extradata;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, extradata, extradata);

  int extradataSize;
  CAST_AVCODEC_GET_MEMBER(AVCodecContext, this->codecContext, extradataSize, extradata_size);

  return copyDataFromRawArray(extradata, extradataSize);
}

} // namespace ffmpeg::avcodec
