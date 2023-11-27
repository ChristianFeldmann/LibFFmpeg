/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/ColorTypes.h>
#include <common/Enums.h>

namespace ffmpeg::internal
{

// Some opaque types so that we can handle pointers to them.
// The implementation depends on the libraries version.
class AVFormatContext;
class AVInputFormat;
class AVPacket;
class AVPacketSideData;
class AVFrame;
class AVFrameSideData;
class AVDictionary;
class AVPixFmtDescriptor;
class AVCodec;
class AVCodecContext;
class AVCodecParameters;
class AVCodecDescriptor;
class AVBufferRef;
class AVStream;
class AVClass;
class AVIOContext;
class AVProgram;
class AVChapter;

/* These enums are only used internally. So far, these enums did not change between the FFmpeg
 * versions. If they ever do change, we will have to add a wrapper around them.
 */

enum AVColorSpace
{
  AVCOL_SPC_RGB                = 0,
  AVCOL_SPC_BT709              = 1,
  AVCOL_SPC_UNSPECIFIED        = 2,
  AVCOL_SPC_RESERVED           = 3,
  AVCOL_SPC_FCC                = 4,
  AVCOL_SPC_BT470BG            = 5,
  AVCOL_SPC_SMPTE170M          = 6,
  AVCOL_SPC_SMPTE240M          = 7,
  AVCOL_SPC_YCOCG              = 8,
  AVCOL_SPC_BT2020_NCL         = 9,
  AVCOL_SPC_BT2020_CL          = 10,
  AVCOL_SPC_SMPTE2085          = 11,
  AVCOL_SPC_CHROMA_DERIVED_NCL = 12,
  AVCOL_SPC_CHROMA_DERIVED_CL  = 13,
  AVCOL_SPC_ICTCP              = 14
};

ColorSpace toColorspace(const AVColorSpace colorspace);

enum AVMediaType
{
  AVMEDIA_TYPE_UNKNOWN = -1, ///< Usually treated as AVMEDIA_TYPE_DATA
  AVMEDIA_TYPE_VIDEO,
  AVMEDIA_TYPE_AUDIO,
  AVMEDIA_TYPE_DATA, ///< Opaque data information usually continuous
  AVMEDIA_TYPE_SUBTITLE,
  AVMEDIA_TYPE_ATTACHMENT, ///< Opaque data information usually sparse
  AVMEDIA_TYPE_NB
};

MediaType   toMediaType(const AVMediaType mediaType);
AVMediaType toAVMediaType(const MediaType mediaType);

enum AVFrameSideDataType
{
  AV_FRAME_DATA_PANSCAN,
  AV_FRAME_DATA_A53_CC,
  AV_FRAME_DATA_STEREO3D,
  AV_FRAME_DATA_MATRIXENCODING,
  AV_FRAME_DATA_DOWNMIX_INFO,
  AV_FRAME_DATA_REPLAYGAIN,
  AV_FRAME_DATA_DISPLAYMATRIX,
  AV_FRAME_DATA_AFD,
  AV_FRAME_DATA_MOTION_VECTORS,
  AV_FRAME_DATA_SKIP_SAMPLES,
  AV_FRAME_DATA_AUDIO_SERVICE_TYPE,
  // The ones below are new in avutil 55
  AV_FRAME_DATA_MASTERING_DISPLAY_METADATA,
  AV_FRAME_DATA_GOP_TIMECODE,
  // These are new in FFmpeg 5
  AV_FRAME_DATA_SPHERICAL,
  AV_FRAME_DATA_CONTENT_LIGHT_LEVEL,
  AV_FRAME_DATA_ICC_PROFILE,
  AV_FRAME_DATA_S12M_TIMECODE,
  AV_FRAME_DATA_DYNAMIC_HDR_PLUS,
  AV_FRAME_DATA_REGIONS_OF_INTEREST,
  AV_FRAME_DATA_VIDEO_ENC_PARAMS,
  AV_FRAME_DATA_SEI_UNREGISTERED,
  AV_FRAME_DATA_FILM_GRAIN_PARAMS,
  AV_FRAME_DATA_DETECTION_BBOXES,
  AV_FRAME_DATA_DOVI_RPU_BUFFER,
  AV_FRAME_DATA_DOVI_METADATA
};

/* The pixel format is very different per FFmpeg version. It actually depends on how FFMpeg
   was compiled. So having a wrapper around this does not make sense. We will only handle
   corresponding AVPixelFormatDescriptors which we can get from FFMpeg.
*/
enum AVPixelFormat
{
  AV_PIX_FMT_NONE = -1
};

/* The order / numbering of the values in this enum changes depending on the libavcodec version
 * number or even how the library was compiled. The id for AV_CODEC_ID_NONE is always 0. Update In
 * ffmpeg 5, this was changed so that the AVCodecID list is fixed per version and does not change
 * depending on compilation.
 */
enum AVCodecID
{
  AV_CODEC_ID_NONE = 0
};

struct AVRational
{
  int num;
  int den;
};

struct AVDictionaryEntry
{
  char *key;
  char *value;
};

} // namespace ffmpeg::internal
