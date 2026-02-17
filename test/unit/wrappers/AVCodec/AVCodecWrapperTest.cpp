/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVCodec/wrappers/AVCodecWrapper.h>
#include <AVCodec/wrappers/AVCodecWrapperInternal.h>
#include <common/InternalTypes.h>
#include <libHandling/FFmpegLibrariesMoc.h>
#include <wrappers/AVCodec/VersionToAVCodecTypes.h>
#include <wrappers/RunTestForAllVersions.h>
#include <wrappers/TestHelper.h>

#include <gtest/gtest.h>

#include <array>

namespace libffmpeg::avcodec
{

namespace
{

using libffmpeg::internal::AVCodec;
using libffmpeg::internal::AVPixelFormat;
using libffmpeg::internal::AVRational;
using ::testing::ElementsAre;
using ::testing::Return;

template <FFmpegVersion V> void runAVCodecWrapperTest()
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(getLibraryVerions(V)));

  constexpr auto TEST_NAME         = "CodecNameTest";
  constexpr auto TEST_LONG_NAME    = "CodecLongNameTest";
  constexpr auto TEST_CODEC_ID     = internal::AV_CODEC_ID_TESTING;
  constexpr auto TEST_CAPABILITIES = 849;

  constexpr uint64_t TEST_CHANNEL_LAYOUT_STEREO      = 0b11;
  constexpr uint64_t TEST_CHANNEL_LAYOUT_5POINT1     = 0b11000001111;
  constexpr uint64_t TEST_CHANNEL_LAYOUT_7POINT1     = 0b11000111111;
  constexpr uint64_t TEST_CHANNEL_LAYOUT_TERMINATION = 0;
  constexpr uint64_t TEST_CHANNEL_FORMATS[4]         = {TEST_CHANNEL_LAYOUT_STEREO,
                                                        TEST_CHANNEL_LAYOUT_5POINT1,
                                                        TEST_CHANNEL_LAYOUT_7POINT1,
                                                        TEST_CHANNEL_LAYOUT_TERMINATION};

  using internal::avcodec::AVChannelLayout;
  using internal::avcodec::AVChannelOrder;
  constexpr AVChannelLayout TEST_AVCHANNEL_LAYOUT_STEREO(
      AVChannelOrder::AV_CHANNEL_ORDER_NATIVE, 2, {TEST_CHANNEL_LAYOUT_STEREO});
  constexpr AVChannelLayout TEST_AVCHANNEL_LAYOUT_5POINT1(
      AVChannelOrder::AV_CHANNEL_ORDER_NATIVE, 2, {TEST_CHANNEL_LAYOUT_5POINT1});
  constexpr AVChannelLayout TEST_AVCHANNEL_LAYOUT_7POINT1(
      AVChannelOrder::AV_CHANNEL_ORDER_NATIVE, 2, {TEST_CHANNEL_LAYOUT_7POINT1});
  constexpr AVChannelLayout TEST_AVCHANNEL_LAYOUT_TERMINATION(
      AVChannelOrder::AV_CHANNEL_ORDER_UNSPEC, 0, {0});
  constexpr AVChannelLayout TEST_AVCHANNEL_LAYOUTS[4] = {TEST_AVCHANNEL_LAYOUT_STEREO,
                                                         TEST_AVCHANNEL_LAYOUT_5POINT1,
                                                         TEST_AVCHANNEL_LAYOUT_7POINT1,
                                                         TEST_AVCHANNEL_LAYOUT_TERMINATION};

  AVCodecType<V> rawCodec;
  rawCodec.name         = TEST_NAME;
  rawCodec.long_name    = TEST_LONG_NAME;
  rawCodec.type         = libffmpeg::internal::AVMEDIA_TYPE_SUBTITLE;
  rawCodec.id           = TEST_CODEC_ID;
  rawCodec.capabilities = TEST_CAPABILITIES;
  if constexpr (V <= FFmpegVersion::FFmpeg_5x)
    rawCodec.channel_layouts = TEST_CHANNEL_FORMATS;
  else
    rawCodec.channel_layouts = TEST_AVCHANNEL_LAYOUTS;

  std::array<AVRational, 4> rawFrameRates = {
      AVRational({30, 1}), AVRational({30, 1001}), AVRational({77, 999}), AVRational({0, 0})};
  rawCodec.supported_framerates = rawFrameRates.data();

  std::vector<AVPixelFormat> rawPixelFormats = {static_cast<AVPixelFormat>(72),
                                                static_cast<AVPixelFormat>(44),
                                                static_cast<AVPixelFormat>(92),
                                                AVPixelFormat::AV_PIX_FMT_NONE};
  rawCodec.pix_fmts                          = rawPixelFormats.data();

  std::vector<int> rawSupportedSampleRates = {22, 44, 55, 0};
  rawCodec.supported_samplerates           = rawSupportedSampleRates.data();

  AVCodecWrapper codec(reinterpret_cast<AVCodec *>(&rawCodec), ffmpegLibraries);

  EXPECT_EQ(codec.getName(), TEST_NAME);
  EXPECT_EQ(codec.getLongName(), TEST_LONG_NAME);
  EXPECT_EQ(codec.getMediaType(), avutil::MediaType::Subtitle);
  EXPECT_EQ(codec.getCodecID(), TEST_CODEC_ID);
  EXPECT_EQ(codec.getCapabilities(), TEST_CAPABILITIES);
  EXPECT_EQ(codec.getSupportedFramerates(),
            toRationalVector(rawFrameRates.begin(), rawFrameRates.end() - 1));
  EXPECT_EQ(codec.getPixelFormats().size(), 3);
  EXPECT_EQ(ffmpegLibraries->functionCallValues.avPixFmtDescGet,
            std::vector(rawPixelFormats.begin(), rawPixelFormats.end() - 1));
  EXPECT_EQ(codec.getSupportedSamplerates(),
            std::vector(rawSupportedSampleRates.begin(), rawSupportedSampleRates.end() - 1));

  const auto channelLayouts = codec.getSupportedChannelLayouts();

  EXPECT_THAT(channelLayouts,
              ElementsAre(std::vector<ChannelInfo>(
                              {{Channel::FrontLeft, {}, "FL"}, {Channel::FrontRight, {}, "FR"}}),
                          std::vector<ChannelInfo>({{Channel::FrontLeft, {}, "FL"},
                                                    {Channel::FrontRight, {}, "FR"},
                                                    {Channel::FrontCenter, {}, "FC"},
                                                    {Channel::LowFrequency, {}, "LFE"},
                                                    {Channel::SideLeft, {}, "SL"},
                                                    {Channel::SideRight, {}, "SR"}}),
                          std::vector<ChannelInfo>({{Channel::FrontLeft, {}, "FL"},
                                                    {Channel::FrontRight, {}, "FR"},
                                                    {Channel::FrontCenter, {}, "FC"},
                                                    {Channel::LowFrequency, {}, "LFE"},
                                                    {Channel::BackLeft, {}, "BL"},
                                                    {Channel::BackRight, {}, "BR"},
                                                    {Channel::SideLeft, {}, "SL"},
                                                    {Channel::SideRight, {}, "SR"}})));
  EXPECT_EQ(channelLayouts.size(), 3);
}

} // namespace

class AVCodecWrapperTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST_F(AVCodecWrapperTest, ConstructorWithNullptrForCodecShouldThrow)
{
  auto     ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  AVCodec *codec           = nullptr;
  EXPECT_THROW(AVCodecWrapper wrapper(codec, ffmpegLibraries), std::runtime_error);
}

TEST_F(AVCodecWrapperTest, ConstructorWithNullptrForFFmpegLibrariesShouldThrow)
{
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries;
  AVDummy                           dummyCodec;
  EXPECT_THROW(AVCodecWrapper wrapper(reinterpret_cast<AVCodec *>(&dummyCodec), ffmpegLibraries),
               std::runtime_error);
}

TEST_P(AVCodecWrapperTest, TestAVCodecWrapper)
{
  const auto version = GetParam();
  RUN_TEST_FOR_VERSION(version, runAVCodecWrapperTest);
}

INSTANTIATE_TEST_SUITE_P(AVCodecWrappers,
                         AVCodecWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace libffmpeg::avcodec
