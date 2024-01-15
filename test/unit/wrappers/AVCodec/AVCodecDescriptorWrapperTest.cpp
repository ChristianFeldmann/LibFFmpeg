/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVCodec/wrappers/AVCodecDescriptorConversion.h>
#include <AVCodec/wrappers/AVCodecDescriptorConversionInternal.h>
#include <common/InternalTypes.h>
#include <wrappers/TestHelper.h>

#include <gtest/gtest.h>

#include <array>

namespace ffmpeg::avcodec
{

using ffmpeg::internal::AVCodecDescriptor;
using ffmpeg::internal::AVCodecID;
using ffmpeg::internal::avcodec::AVCodecDescriptor_56;
using ffmpeg::internal::avcodec::AVCodecDescriptor_57;
using ffmpeg::internal::avcodec::AVProfile_57;

namespace
{

constexpr auto TEST_NAME      = "TestName";
constexpr auto TEST_LONG_NAME = "TestLongName";

} // namespace

class AVCodecDescriptorConversionTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST_F(AVCodecDescriptorConversionTest, ConstructorWithNullptrShoudlTrhwo)
{
  const AVCodecDescriptor *nullDescriptor = nullptr;
  EXPECT_THROW(convertAVCodecDescriptor(reinterpret_cast<AVCodecDescriptor *>(&nullDescriptor), 56),
               std::runtime_error);
}

template <typename AVCodecDescriptorType> void runParsingTest()
{
  AVCodecDescriptorType rawDescriptor;
  rawDescriptor.id        = static_cast<AVCodecID>(37);
  rawDescriptor.type      = ffmpeg::internal::AVMEDIA_TYPE_VIDEO;
  rawDescriptor.name      = TEST_NAME;
  rawDescriptor.long_name = TEST_LONG_NAME;

  constexpr auto AV_CODEC_PROP_LOSSY      = (1 << 1);
  constexpr auto AV_CODEC_PROP_BITMAP_SUB = (1 << 16);
  rawDescriptor.props                     = AV_CODEC_PROP_LOSSY + AV_CODEC_PROP_BITMAP_SUB;

  const std::array<const char *, 4> MIME_TYPES = {"MimeType0", "MimeType1", "MimeType2", nullptr};
  rawDescriptor.mime_types                     = MIME_TYPES.data();

  std::array<AVProfile_57, 4>       profiles;
  const std::array<const char *, 4> PROFILE_NAMES = {
      "ProfileName0", "ProfileName1", "ProfileName2", "TerminatingProfile"};
  constexpr auto FF_PROFILE_UNKNOWN = -99;
  for (int i = 0; i < 4; ++i)
  {
    profiles[i].profile = (i == 3) ? FF_PROFILE_UNKNOWN : i * 22;
    profiles[i].name    = PROFILE_NAMES.at(i);
  }

  Version version(56);
  if constexpr (std::is_same_v<AVCodecDescriptorType, AVCodecDescriptor_57>)
  {
    rawDescriptor.profiles = profiles.data();
    version                = Version(57);
  }

  const auto descriptor =
      convertAVCodecDescriptor(reinterpret_cast<AVCodecDescriptor *>(&rawDescriptor), version);

  EXPECT_EQ(descriptor.mediaType, avutil::MediaType::Video);
  EXPECT_EQ(descriptor.codecName, TEST_NAME);
  EXPECT_EQ(descriptor.longName, TEST_LONG_NAME);

  CodecDescriptorProperties expectedProperties;
  expectedProperties.lossy     = true;
  expectedProperties.bitmapSub = true;

  EXPECT_EQ(descriptor.properties, expectedProperties);

  const auto expectedMimeTypes = std::vector<std::string>({"MimeType0", "MimeType1", "MimeType2"});
  EXPECT_EQ(descriptor.mimeTypes, expectedMimeTypes);

  if constexpr (std::is_same_v<AVCodecDescriptorType, AVCodecDescriptor_57>)
  {
    const auto expectedProfiles =
        std::vector<std::string>({"ProfileName0", "ProfileName1", "ProfileName2"});
    EXPECT_EQ(descriptor.profiles, expectedProfiles);
  }
}

TEST_F(AVCodecDescriptorConversionTest, TestParsing_AVCodecVersion_56)
{
  runParsingTest<AVCodecDescriptor_56>();
}

TEST_F(AVCodecDescriptorConversionTest, TestParsing_AVCodecVersion_greater_56)
{
  runParsingTest<AVCodecDescriptor_57>();
}

} // namespace ffmpeg::avcodec
