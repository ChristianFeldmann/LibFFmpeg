/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <stdint.h>

namespace ffmpeg::internal
{

// Some opaque types so that we can handle pointers to them.
// The implementation depends on the libraries version.
class AVBufferRef;
class AVChapter;
class AVClass;
class AVCodec;
class AVCodecContext;
class AVCodecDescriptor;
class AVCodecInternal;
class AVCodecParameters;
class AVDictionary;
class AVFormatContext;
class AVFrame;
class AVFrameSideData;
class AVInputFormat;
class AVIOContext;
class AVOutputFormat;
class AVPacket;
class AVPacketSideData;
class AVPixFmtDescriptor;
class AVProfile;
class AVProgram;
class AVStream;
class AVStreamGroup;

#define AV_NUM_DATA_POINTERS 8

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

enum AVColorPrimaries
{
  AVCOL_PRI_RESERVED0   = 0,
  AVCOL_PRI_BT709       = 1, ///< also ITU-R BT1361 / IEC 61966-2-4 / SMPTE RP177 Annex B
  AVCOL_PRI_UNSPECIFIED = 2,
  AVCOL_PRI_RESERVED    = 3,
  AVCOL_PRI_BT470M      = 4, ///< also FCC Title 47 Code of Federal Regulations 73.682 (a)(20)

  AVCOL_PRI_BT470BG =
      5, ///< also ITU-R BT601-6 625 / ITU-R BT1358 625 / ITU-R BT1700 625 PAL & SECAM
  AVCOL_PRI_SMPTE170M    = 6,  ///< also ITU-R BT601-6 525 / ITU-R BT1358 525 / ITU-R BT1700 NTSC
  AVCOL_PRI_SMPTE240M    = 7,  ///< functionally identical to above
  AVCOL_PRI_FILM         = 8,  ///< colour filters using Illuminant C
  AVCOL_PRI_BT2020       = 9,  ///< ITU-R BT2020
  AVCOL_PRI_SMPTE428     = 10, ///< SMPTE ST 428-1 (CIE 1931 XYZ)
  AVCOL_PRI_SMPTEST428_1 = AVCOL_PRI_SMPTE428,
  AVCOL_PRI_SMPTE431     = 11, ///< SMPTE ST 431-2 (2011)
  AVCOL_PRI_SMPTE432     = 12, ///< SMPTE ST 432-1 D65 (2010)
  AVCOL_PRI_EBU3213   = 22, ///< EBU Tech. 3213-E (nothing there) / one of JEDEC P22 group phosphors
  AVCOL_PRI_JEDEC_P22 = AVCOL_PRI_EBU3213
};

enum AVColorTransferCharacteristic
{
  AVCOL_TRC_RESERVED0   = 0,
  AVCOL_TRC_BT709       = 1, ///< also ITU-R BT1361
  AVCOL_TRC_UNSPECIFIED = 2,
  AVCOL_TRC_RESERVED    = 3,
  AVCOL_TRC_GAMMA22     = 4, ///< also ITU-R BT470M / ITU-R BT1700 625 PAL & SECAM
  AVCOL_TRC_GAMMA28     = 5, ///< also ITU-R BT470BG
  AVCOL_TRC_SMPTE170M =
      6, ///< also ITU-R BT601-6 525 or 625 / ITU-R BT1358 525 or 625 / ITU-R BT1700 NTSC
  AVCOL_TRC_SMPTE240M    = 7,
  AVCOL_TRC_LINEAR       = 8,  ///< "Linear transfer characteristics"
  AVCOL_TRC_LOG          = 9,  ///< "Logarithmic transfer characteristic (100:1 range)"
  AVCOL_TRC_LOG_SQRT     = 10, ///< "Logarithmic transfer characteristic (100 * Sqrt(10) : 1 range)"
  AVCOL_TRC_IEC61966_2_4 = 11, ///< IEC 61966-2-4
  AVCOL_TRC_BT1361_ECG   = 12, ///< ITU-R BT1361 Extended Colour Gamut
  AVCOL_TRC_IEC61966_2_1 = 13, ///< IEC 61966-2-1 (sRGB or sYCC)
  AVCOL_TRC_BT2020_10    = 14, ///< ITU-R BT2020 for 10-bit system
  AVCOL_TRC_BT2020_12    = 15, ///< ITU-R BT2020 for 12-bit system
  AVCOL_TRC_SMPTE2084    = 16, ///< SMPTE ST 2084 for 10-, 12-, 14- and 16-bit systems
  AVCOL_TRC_SMPTEST2084  = AVCOL_TRC_SMPTE2084,
  AVCOL_TRC_SMPTE428     = 17, ///< SMPTE ST 428-1
  AVCOL_TRC_SMPTEST428_1 = AVCOL_TRC_SMPTE428,
  AVCOL_TRC_ARIB_STD_B67 = 18 ///< ARIB STD-B67, known as "Hybrid log-gamma"
};

enum AVColorRange
{
  AVCOL_RANGE_UNSPECIFIED = 0,
  AVCOL_RANGE_MPEG        = 1, ///< the normal 219*2^(n-8) "MPEG" YUV ranges
  AVCOL_RANGE_JPEG        = 2, ///< the normal     2^n-1   "JPEG" YUV ranges
};

/**
 * Location of chroma samples.
 *
 * Illustration showing the location of the first (top left) chroma sample of the
 * image, the left shows only luma, the right
 * shows the location of the chroma sample, the 2 could be imagined to overlay
 * each other but are drawn separately due to limitations of ASCII
 *
 *                1st 2nd       1st 2nd horizontal luma sample positions
 *                 v   v         v   v
 *                 ______        ______
 *1st luma line > |X   X ...    |3 4 X ...     X are luma samples,
 *                |             |1 2           1-6 are possible chroma positions
 *2nd luma line > |X   X ...    |5 6 X ...     0 is undefined/unknown position
 */
enum AVChromaLocation
{
  AVCHROMA_LOC_UNSPECIFIED = 0,
  AVCHROMA_LOC_LEFT        = 1, ///< MPEG-2/4 4:2:0, H.264 default for 4:2:0
  AVCHROMA_LOC_CENTER      = 2, ///< MPEG-1 4:2:0, JPEG 4:2:0, H.263 4:2:0
  AVCHROMA_LOC_TOPLEFT     = 3, ///< ITU-R 601, SMPTE 274M 296M S314M(DV 4:1:1), mpeg2 4:2:2
  AVCHROMA_LOC_TOP         = 4,
  AVCHROMA_LOC_BOTTOMLEFT  = 5,
  AVCHROMA_LOC_BOTTOM      = 6
};

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

enum AVPictureType
{
  AV_PICTURE_TYPE_NONE = 0, ///< Undefined
  AV_PICTURE_TYPE_I,        ///< Intra
  AV_PICTURE_TYPE_P,        ///< Predicted
  AV_PICTURE_TYPE_B,        ///< Bi-dir predicted
  AV_PICTURE_TYPE_S,        ///< S(GMC)-VOP MPEG4
  AV_PICTURE_TYPE_SI,       ///< Switching Intra
  AV_PICTURE_TYPE_SP,       ///< Switching Predicted
  AV_PICTURE_TYPE_BI,       ///< BI type
};

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

enum AVDiscard
{
  /* We leave some space between them for extensions (drop some
   * keyframes for intra-only or drop just some bidir frames). */
  AVDISCARD_NONE     = -16, ///< discard nothing
  AVDISCARD_DEFAULT  = 0,   ///< discard useless packets like 0 size packets in avi
  AVDISCARD_NONREF   = 8,   ///< discard all non reference
  AVDISCARD_BIDIR    = 16,  ///< discard all bidirectional frames
  AVDISCARD_NONINTRA = 24,  ///< discard all non intra frames
  AVDISCARD_NONKEY   = 32,  ///< discard all frames except keyframes
  AVDISCARD_ALL      = 48,  ///< discard all
};

enum AVStreamParseType
{
  AVSTREAM_PARSE_NONE,
  AVSTREAM_PARSE_FULL,       /**< full parsing and repack */
  AVSTREAM_PARSE_HEADERS,    /**< Only parse headers, do not repack. */
  AVSTREAM_PARSE_TIMESTAMPS, /**< full parsing and interpolation of timestamps for frames not
                                starting on a packet boundary */
  AVSTREAM_PARSE_FULL_ONCE,  /**< full parsing and repack of the first frame only, only implemented
                                for H.264 currently */
  AVSTREAM_PARSE_FULL_RAW,   /**< full parsing and repack with timestamp and position generation by
                                parser for raw   this assumes that each packet in the file contains no
                                demuxer level headers and   just codec level data, otherwise position
                                generation would fail */
};

enum AVFieldOrder
{
  AV_FIELD_UNKNOWN,
  AV_FIELD_PROGRESSIVE,
  AV_FIELD_TT, //< Top coded_first, top displayed first
  AV_FIELD_BB, //< Bottom coded first, bottom displayed first
  AV_FIELD_TB, //< Top coded first, bottom displayed first
  AV_FIELD_BT, //< Bottom coded first, top displayed first
};

enum AVSampleFormat
{
  AV_SAMPLE_FMT_NONE = -1,
  AV_SAMPLE_FMT_U8,  ///< unsigned 8 bits
  AV_SAMPLE_FMT_S16, ///< signed 16 bits
  AV_SAMPLE_FMT_S32, ///< signed 32 bits
  AV_SAMPLE_FMT_FLT, ///< float
  AV_SAMPLE_FMT_DBL, ///< double

  AV_SAMPLE_FMT_U8P,  ///< unsigned 8 bits, planar
  AV_SAMPLE_FMT_S16P, ///< signed 16 bits, planar
  AV_SAMPLE_FMT_S32P, ///< signed 32 bits, planar
  AV_SAMPLE_FMT_FLTP, ///< float, planar
  AV_SAMPLE_FMT_DBLP, ///< double, planar
  AV_SAMPLE_FMT_S64,  ///< signed 64 bits
  AV_SAMPLE_FMT_S64P, ///< signed 64 bits, planar
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

struct AVFrac
{
  int64_t val, num, den;
};

struct AVDictionaryEntry
{
  char *key;
  char *value;
};

} // namespace ffmpeg::internal
