/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVCodec/wrappers/AVCodecWrapper.h>
#include <AVCodec/wrappers/AVCodecWrapperInternal.h>
#include <common/InternalTypes.h>
#include <wrappers/TestHelper.h>

#include <libHandling/FFmpegLibrariesMoc.h>

#include <gtest/gtest.h>

namespace ffmpeg::avcodec
{

namespace
{

using ffmpeg::internal::AVCodec;
using ffmpeg::internal::AVPixelFormat;
using ffmpeg::internal::AVRational;
using ffmpeg::internal::avcodec::AVCodec_56;
using ffmpeg::internal::avcodec::AVCodec_57;
using ffmpeg::internal::avcodec::AVCodec_58;
using ffmpeg::internal::avcodec::AVCodec_59;
using ffmpeg::internal::avcodec::AVCodec_60;
using ::testing::Return;

template <typename AVCodecType> void runAVCodecWrapperTest(const LibraryVersions &version)
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  std::vector<AVPixelFormat> descriptionGetCalls;
  ffmpegLibraries->avutil.av_pix_fmt_desc_get = [&descriptionGetCalls](AVPixelFormat pix_fmt)
  {
    descriptionGetCalls.push_back(pix_fmt);
    return nullptr;
  };

  constexpr auto TEST_NAME         = "CodecNameTest";
  constexpr auto TEST_LONG_NAME    = "CodecLongNameTest";
  constexpr auto TEST_CODEC_ID     = static_cast<ffmpeg::internal::AVCodecID>(98);
  constexpr auto TEST_CAPABILITIES = 849;

  AVCodecType rawCodec;
  rawCodec.name         = TEST_NAME;
  rawCodec.long_name    = TEST_LONG_NAME;
  rawCodec.type         = ffmpeg::internal::AVMEDIA_TYPE_SUBTITLE;
  rawCodec.id           = TEST_CODEC_ID;
  rawCodec.capabilities = TEST_CAPABILITIES;

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
  EXPECT_EQ(descriptionGetCalls, std::vector(rawPixelFormats.begin(), rawPixelFormats.end() - 1));
  EXPECT_EQ(codec.getSupportedSamplerates(),
            std::vector(rawSupportedSampleRates.begin(), rawSupportedSampleRates.end() - 1));
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
  AVCodec_56                        codec;
  EXPECT_THROW(AVCodecWrapper wrapper(reinterpret_cast<AVCodec *>(&codec), ffmpegLibraries),
               std::runtime_error);
}

TEST_P(AVCodecWrapperTest, TestAVCodecWrapper)
{
  const auto version = GetParam();
  if (version.avformat.major == 56)
    runAVCodecWrapperTest<AVCodec_56>(version);
  else if (version.avformat.major == 57)
    runAVCodecWrapperTest<AVCodec_57>(version);
  else if (version.avformat.major == 58)
    runAVCodecWrapperTest<AVCodec_58>(version);
  else if (version.avformat.major == 59)
    runAVCodecWrapperTest<AVCodec_59>(version);
  else if (version.avformat.major == 60)
    runAVCodecWrapperTest<AVCodec_60>(version);
}

INSTANTIATE_TEST_SUITE_P(AVCodecWrappers,
                         AVCodecWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace ffmpeg::avcodec
