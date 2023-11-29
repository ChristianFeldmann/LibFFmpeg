/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/Version.h>
#include <libHandling/IFFmpegLibraries.h>

#include <gmock/gmock.h>

namespace ffmpeg
{

constexpr auto FFMPEG_VERSION_6 =
    LibraryVersions({Version(60), Version(60), Version(58), Version(4)});

class FFmpegLibrariesMock : public IFFmpegLibraries
{
public:
  MOCK_METHOD(std::vector<LibraryInfo>, getLibrariesInfo, (), (const, override));
  MOCK_METHOD(LibraryVersions, getLibrariesVersion, (), (const, override));

  MOCK_METHOD(std::string_view, getLogList, (), (const, override));
};

} // namespace ffmpeg
