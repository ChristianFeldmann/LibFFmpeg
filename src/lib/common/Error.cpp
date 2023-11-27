/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Error.h"

namespace ffmpeg
{

namespace
{

constexpr int makeErrorTag(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
  return -(static_cast<int>(a) | (static_cast<int>(b) << 8) | (static_cast<int>(c) << 16) |
           (static_cast<unsigned>(d) << 24));
}

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

} // namespace

ReturnCode toReturnCode(const int returnValue)
{
  if (returnValue >= 0)
    return ReturnCode::Ok;

  switch (returnValue)
  {
  case -11:
    return ReturnCode::TryAgain;
  case -1179861752:
    return ReturnCode::BSFNotFound;
  case -558323010:
    return ReturnCode::Bug;
  case -1397118274:
    return ReturnCode::BufferTooSmall;
  case -1128613112:
    return ReturnCode::DecoderNotFound;
  case -1296385272:
    return ReturnCode::DemuxerNotFound;
  case -1129203192:
    return ReturnCode::EncoderNotFound;
  case -541478725:
    return ReturnCode::EndOfFile;
  case -1414092869:
    return ReturnCode::Exit;
  case -542398533:
    return ReturnCode::External;
  case -1279870712:
    return ReturnCode::FilterNotFound;
  case -1094995529:
    return ReturnCode::InvalidData;
  case -1481985528:
    return ReturnCode::MuxerNotFound;
  case -1414549496:
    return ReturnCode::OptionNotFound;
  case -1163346256:
    return ReturnCode::NotImplementedYet;
  case -1330794744:
    return ReturnCode::ProtocolNotFound;
  case -1381258232:
    return ReturnCode::StreamNotFound;
  case -541545794:
    return ReturnCode::Bug;
  case -1313558101:
    return ReturnCode::Unknown;
  case -0x2bb2afa8:
    return ReturnCode::Experimental;
  case -0x636e6701:
    return ReturnCode::InputChanged;
  case -0x636e6702:
    return ReturnCode::OutputChanged;
  case -808465656:
    return ReturnCode::HttpBadRequest;
  case -825242872:
    return ReturnCode::HttpUnauthorized;
  case -858797304:
    return ReturnCode::HttpForbidden;
  case -875574520:
    return ReturnCode::HttpNotFound;
  case -1482175736:
    return ReturnCode::HttpOther4xx;
  case -1482175992:
    return ReturnCode::HttpServerError;

  default:
    return ReturnCode::Unknown;
  }
}

} // namespace ffmpeg
