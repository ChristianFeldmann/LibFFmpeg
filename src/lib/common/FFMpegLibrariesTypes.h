/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <errno.h>
#include <optional>
#include <sstream>
#include <stdint.h>
#include <vector>

/* This file defines all FFmpeg specific defines like structs, enums and some common things like
 * Rational. These things do not change when a new major version of the libraries is released. (If
 * they do, they will have to be moved to the FFmpegVersionHandler). All defines in here were taken
 * from the FFmpeg public API headers. Please see (www.ffmpeg.org).
 */

namespace ffmpeg
{

#define AV_LOG_WARNING 24

#define AV_NUM_DATA_POINTERS 8
#define AV_TIME_BASE 1000000
#define AV_INPUT_BUFFER_PADDING_SIZE 32

// How to construct error tags
#define MKTAG(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))
#define FFERRTAG(a, b, c, d) (-(int)MKTAG(a, b, c, d))

#if EDOM > 0
#define AVERROR(e) (-(e))
#define AVUNERROR(e) (-(e))
#else
/* Some platforms have E* and errno already negated. */
#define AVERROR(e) (e)
#define AVUNERROR(e) (e)
#endif

// Some error tags
#define AVERROR_BSF_NOT_FOUND FFERRTAG(0xF8, 'B', 'S', 'F') ///< Bitstream filter not found
#define AVERROR_BUG FFERRTAG('B', 'U', 'G', '!')            ///< Internal bug, also see AVERROR_BUG2
#define AVERROR_BUFFER_TOO_SMALL FFERRTAG('B', 'U', 'F', 'S')   ///< Buffer too small
#define AVERROR_DECODER_NOT_FOUND FFERRTAG(0xF8, 'D', 'E', 'C') ///< Decoder not found
#define AVERROR_DEMUXER_NOT_FOUND FFERRTAG(0xF8, 'D', 'E', 'M') ///< Demuxer not found
#define AVERROR_ENCODER_NOT_FOUND FFERRTAG(0xF8, 'E', 'N', 'C') ///< Encoder not found
#define AVERROR_EOF FFERRTAG('E', 'O', 'F', ' ')                ///< End of file
#define AVERROR_EXIT                                                                               \
  FFERRTAG('E',                                                                                    \
           'X',                                                                                    \
           'I',                                                                                    \
           'T') ///< Immediate exit was requested; the called function should not be restarted
#define AVERROR_EXTERNAL FFERRTAG('E', 'X', 'T', ' ') ///< Generic error in an external library
#define AVERROR_FILTER_NOT_FOUND FFERRTAG(0xF8, 'F', 'I', 'L') ///< Filter not found
#define AVERROR_INVALIDDATA                                                                        \
  FFERRTAG('I', 'N', 'D', 'A') ///< Invalid data found when processing input
#define AVERROR_MUXER_NOT_FOUND FFERRTAG(0xF8, 'M', 'U', 'X')  ///< Muxer not found
#define AVERROR_OPTION_NOT_FOUND FFERRTAG(0xF8, 'O', 'P', 'T') ///< Option not found
#define AVERROR_PATCHWELCOME                                                                       \
  FFERRTAG('P', 'A', 'W', 'E') ///< Not yet implemented in FFmpeg, patches welcome
#define AVERROR_PROTOCOL_NOT_FOUND FFERRTAG(0xF8, 'P', 'R', 'O') ///< Protocol not found

#define AVSEEK_FLAG_BACKWARD 1 ///< seek backward
#define AVSEEK_FLAG_BYTE 2     ///< seeking based on position in bytes
#define AVSEEK_FLAG_ANY 4      ///< seek to any frame, even non-keyframes
#define AVSEEK_FLAG_FRAME 8    ///< seeking based on frame number

#define AV_NOPTS_VALUE ((int64_t)UINT64_C(0x8000000000000000))

// The exact value of the fractional number is: 'val + num / den'. num is assumed to be 0 <= num <
// den.
struct AVFrac
{
  int64_t val, num, den;
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

#define AV_PKT_FLAG_KEY 0x0001     ///< The packet contains a keyframe
#define AV_PKT_FLAG_CORRUPT 0x0002 ///< The packet content is corrupted
#define AV_PKT_FLAG_DISCARD 0x0004 ///< Not required for output and should be discarded

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

} // namespace ffmpeg
