/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "Error.h"

#include <algorithm>
#include <array>

namespace libffmpeg
{

namespace
{

using AVErrorToReturnCode = std::pair<int, ReturnCode>;

#if (defined(__APPLE__))
constexpr int EAGAIN_VALUE = 35;
#else
constexpr int EAGAIN_VALUE = 11;
#endif

constexpr std::array<AVErrorToReturnCode, 28> AVERRORToReturnCodeMap{
    {{-EAGAIN_VALUE, ReturnCode::TryAgain},
     {-1179861752, ReturnCode::BSFNotFound},
     {-558323010, ReturnCode::Bug},
     {-1397118274, ReturnCode::BufferTooSmall},
     {-1128613112, ReturnCode::DecoderNotFound},
     {-1296385272, ReturnCode::DemuxerNotFound},
     {-1129203192, ReturnCode::EncoderNotFound},
     {-541478725, ReturnCode::EndOfFile},
     {-1414092869, ReturnCode::Exit},
     {-542398533, ReturnCode::External},
     {-1279870712, ReturnCode::FilterNotFound},
     {-1094995529, ReturnCode::InvalidData},
     {-1481985528, ReturnCode::MuxerNotFound},
     {-1414549496, ReturnCode::OptionNotFound},
     {-1163346256, ReturnCode::NotImplementedYet},
     {-1330794744, ReturnCode::ProtocolNotFound},
     {-1381258232, ReturnCode::StreamNotFound},
     {-541545794, ReturnCode::Bug},
     {-1313558101, ReturnCode::Unknown},
     {-0x2bb2afa8, ReturnCode::Experimental},
     {-0x636e6701, ReturnCode::InputChanged},
     {-0x636e6702, ReturnCode::OutputChanged},
     {-808465656, ReturnCode::HttpBadRequest},
     {-825242872, ReturnCode::HttpUnauthorized},
     {-858797304, ReturnCode::HttpForbidden},
     {-875574520, ReturnCode::HttpNotFound},
     {-1482175736, ReturnCode::HttpOther4xx},
     {-1482175992, ReturnCode::HttpServerError}}};


} // namespace

ReturnCode toReturnCode(const int returnValue)
{
  if (returnValue >= 0)
    return ReturnCode::Ok;

  const auto result = std::find_if(AVERRORToReturnCodeMap.begin(),
                                   AVERRORToReturnCodeMap.end(),
                                   [returnValue](AVErrorToReturnCode errorToReturnCode) {
                                     return errorToReturnCode.first == returnValue;
                                   });
  if (result != AVERRORToReturnCodeMap.end())
    return result->second;

  return ReturnCode::Unknown;
}

int toAVError(const ReturnCode returnCode)
{
  if (returnCode == ReturnCode::Ok)
    return 0;

  const auto result = std::find_if(AVERRORToReturnCodeMap.begin(),
                                   AVERRORToReturnCodeMap.end(),
                                   [returnCode](AVErrorToReturnCode errorToReturnCode) {
                                     return errorToReturnCode.second == returnCode;
                                   });
  if (result != AVERRORToReturnCodeMap.end())
    return result->first;

  constexpr auto avErrorUnknown = -1313558101;
  return avErrorUnknown;
}

} // namespace libffmpeg
