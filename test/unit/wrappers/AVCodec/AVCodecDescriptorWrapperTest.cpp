/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVCodec/wrappers/AVCodecDescriptorWrapper.h>
#include <AVCodec/wrappers/AVCodecDescriptorWrapperInternal.h>
#include <common/InternalTypes.h>
#include <wrappers/TestHelper.h>

#include <gtest/gtest.h>

#include <array>

namespace ffmpeg::avcodec
{

using ffmpeg::internal::AVCodecDescriptor;
using ffmpeg::internal::AVCodecID;
using ffmpeg::internal::avcodec::AVCodecDescriptor_56;
using ffmpeg::internal::avcodec::AVProfile_56;

class AVCodecDescriptorWrapperTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST_F(AVCodecDescriptorWrapperTest, ConstructorWithNullptrShoudlTrhwo)
{
  const AVCodecDescriptor *nullDescriptor = nullptr;
  EXPECT_THROW(AVCodecDescriptorWrapper wrapper(nullDescriptor), std::runtime_error);
}

TEST_F(AVCodecDescriptorWrapperTest, TestParsing)
{
  constexpr auto TEST_NAME      = "TestName";
  constexpr auto TEST_LONG_NAME = "TestLongName";

  AVCodecDescriptor_56 rawDescriptor;
  rawDescriptor.id        = static_cast<AVCodecID>(37);
  rawDescriptor.type      = ffmpeg::internal::AVMEDIA_TYPE_VIDEO;
  rawDescriptor.name      = TEST_NAME;
  rawDescriptor.long_name = TEST_LONG_NAME;

  constexpr auto AV_CODEC_PROP_LOSSY      = (1 << 1);
  constexpr auto AV_CODEC_PROP_BITMAP_SUB = (1 << 16);
  rawDescriptor.props                     = AV_CODEC_PROP_LOSSY + AV_CODEC_PROP_BITMAP_SUB;

  std::array<AVProfile_56, 4>       profiles;
  const std::array<const char *, 4> PROFILE_NAMES = {
      "ProfileName0", "ProfileName1", "ProfileName2", "TerminatingProfile"};
  constexpr auto FF_PROFILE_UNKNOWN = -99;
  for (int i = 0; i < 4; ++i)
  {
    profiles[i].profile = (i == 3) ? FF_PROFILE_UNKNOWN : i * 22;
    profiles[i].name    = PROFILE_NAMES.at(i);
  }
  rawDescriptor.profiles = profiles.data();

  const std::array<const char *, 4> MIME_TYPES = {"MimeType0", "MimeType1", "MimeType2", nullptr};
  rawDescriptor.mime_types                     = MIME_TYPES.data();

  AVCodecDescriptorWrapper descriptor(reinterpret_cast<AVCodecDescriptor *>(&rawDescriptor));
  EXPECT_EQ(descriptor.getMediaType(), avutil::MediaType::Video);
  EXPECT_EQ(descriptor.getCodecName(), TEST_NAME);
  EXPECT_EQ(descriptor.getLongName(), TEST_LONG_NAME);

  AVCodecDescriptorProperties expectedProperties;
  expectedProperties.lossy     = true;
  expectedProperties.bitmapSub = true;

  EXPECT_EQ(descriptor.getProperties(), expectedProperties);

  const auto expectedMimeTypes = std::vector<std::string>({"MimeType0", "MimeType1", "MimeType2"});
  EXPECT_EQ(descriptor.getMimeTypes(), expectedMimeTypes);

  const auto expectedProfiles =
      std::vector<std::string>({"ProfileName0", "ProfileName1", "ProfileName2"});
  EXPECT_EQ(descriptor.getProfiles(), expectedProfiles);
}

} // namespace ffmpeg::avcodec
