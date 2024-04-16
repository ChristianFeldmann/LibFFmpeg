/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVCodec/wrappers/AVCodecContextWrapper.h>
#include <AVCodec/wrappers/AVPacketWrapperInternal.h>
#include <AVFormat/wrappers/AVCodecParametersWrapper.h>
#include <common/InternalTypes.h>
#include <libHandling/FFmpegLibrariesMoc.h>
#include <wrappers/AVCodec/VersionToAVCodecTypes.h>
#include <wrappers/RunTestForAllVersions.h>
#include <wrappers/TestHelper.h>

#include <gtest/gtest.h>

#include <array>

namespace ffmpeg::avcodec
{

using ffmpeg::internal::AVCodec;
using ffmpeg::internal::AVCodecContext;
using ffmpeg::internal::AVCodecID;
using ffmpeg::internal::AVCodecParameters;
using ffmpeg::internal::AVColorSpace;
using ffmpeg::internal::AVFrame;
using ffmpeg::internal::AVPacket;
using ffmpeg::internal::AVPixelFormat;
using ffmpeg::internal::AVRational;
using ::testing::NiceMock;
using ::testing::Return;

template <FFmpegVersion V> void runAVCodecContextTest()
{
  constexpr auto TEST_CODEC_ID     = internal::AV_CODEC_ID_TESTING;
  constexpr auto TEST_PIXEL_FORMAT = internal::AV_PIX_FMT_TESTING;
  constexpr auto TEST_WIDTH        = 728;
  constexpr auto TEST_HEIGHT       = 20;
  constexpr auto TEST_TIMEBASE     = AVRational({12, 44});

  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(getLibraryVerions(V)));

  ffmpegLibraries->functionChecks.avutilPixFmtDescGetExpectedFormat = TEST_PIXEL_FORMAT;

  AVCodecContextType<V> codecContext;
  codecContext.codec_type = ffmpeg::internal::AVMEDIA_TYPE_ATTACHMENT;
  codecContext.codec_id   = TEST_CODEC_ID;
  codecContext.pix_fmt    = TEST_PIXEL_FORMAT;
  codecContext.width      = TEST_WIDTH;
  codecContext.height     = TEST_HEIGHT;
  codecContext.colorspace = AVColorSpace::AVCOL_SPC_BT2020_NCL;
  codecContext.time_base  = TEST_TIMEBASE;

  std::array<uint8_t, 4> TEST_EXTRADATA = {22, 56, 19, 22};
  codecContext.extradata                = TEST_EXTRADATA.data();
  codecContext.extradata_size           = static_cast<int>(TEST_EXTRADATA.size());

  AVCodecContextWrapper wrapper(reinterpret_cast<AVCodecContext *>(&codecContext), ffmpegLibraries);

  EXPECT_EQ(wrapper.getCodecType(), avutil::MediaType::Attachment);
  EXPECT_EQ(wrapper.getCodecID(), TEST_CODEC_ID);
  EXPECT_EQ(wrapper.getPixelFormat().name, "None");
  EXPECT_EQ(ffmpegLibraries->functionCounters.avPixFmtDescGet, 1);
  EXPECT_EQ(wrapper.getSize(), Size({TEST_WIDTH, TEST_HEIGHT}));
  EXPECT_EQ(wrapper.getColorspace(), avutil::ColorSpace::BT2020_NCL);
  EXPECT_EQ(wrapper.getTimeBase(), Rational({TEST_TIMEBASE.num, TEST_TIMEBASE.den}));
  EXPECT_EQ(wrapper.getExtradata(), dataArrayToByteVector(TEST_EXTRADATA));
}

class AVCodecContextWrapperTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST_F(AVCodecContextWrapperTest, ConstructorWithNullptrForCodecContextShouldThrow)
{
  auto            ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  AVCodecContext *codecContext{};
  EXPECT_THROW(AVCodecContextWrapper wrapper(codecContext, ffmpegLibraries), std::runtime_error);
}

TEST_F(AVCodecContextWrapperTest, ConstructorWithNullptrForFFmpegLibrariesShouldThrow)
{
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries;
  AVDummy                           codecContext;
  EXPECT_THROW(AVCodecContextWrapper wrapper(reinterpret_cast<AVCodecContext *>(&codecContext),
                                             ffmpegLibraries),
               std::runtime_error);
}

TEST_F(AVCodecContextWrapperTest, TestOpeningOfFile)
{
  auto ffmpegLibraries = std::make_shared<NiceMock<FFmpegLibrariesMock>>();

  {
    AVDummy                            codecParametersDummy;
    avformat::AVCodecParametersWrapper codecParameters(
        reinterpret_cast<AVCodecParameters *>(&codecParametersDummy), ffmpegLibraries);

    auto context = AVCodecContextWrapper(ffmpegLibraries);
    EXPECT_TRUE(context.openContextForDecoding(codecParameters));
  }

  EXPECT_EQ(ffmpegLibraries->functionCounters.avcodecAllocContext3, 1);
  EXPECT_EQ(ffmpegLibraries->functionCounters.avcodecParametersToContext, 1);
  EXPECT_EQ(ffmpegLibraries->functionCounters.avcodecOpen2, 1);
}

TEST_F(AVCodecContextWrapperTest, TestSendingPackets)
{
  auto ffmpegLibraries = std::make_shared<NiceMock<FFmpegLibrariesMock>>();

  AVDummy               codecContext;
  AVCodecContextWrapper wrapper(reinterpret_cast<AVCodecContext *>(&codecContext), ffmpegLibraries);

  AVPacketWrapper packet(ffmpegLibraries);

  wrapper.sendPacket(packet);
  wrapper.sendPacket(packet);
  wrapper.sendFlushPacket();

  EXPECT_EQ(ffmpegLibraries->functionCounters.avcodecSendPacketNonNull, 2);
  EXPECT_EQ(ffmpegLibraries->functionCounters.avcodecSendPacketNull, 1);
}

TEST_F(AVCodecContextWrapperTest, TestRecievingFrames)
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();

  AVDummy               codecContext;
  AVCodecContextWrapper wrapper(reinterpret_cast<AVCodecContext *>(&codecContext), ffmpegLibraries);

  {
    auto [frame, returnCode] = wrapper.revieveFrame();
    EXPECT_EQ(returnCode, ReturnCode::Ok);
    EXPECT_EQ(ffmpegLibraries->functionCounters.avcodecReceiveFrame, 1);
  }

  {
    auto [frame, returnCode] = wrapper.revieveFrame();
    EXPECT_EQ(returnCode, ReturnCode::Ok);
    EXPECT_EQ(ffmpegLibraries->functionCounters.avcodecReceiveFrame, 2);
  }
}

TEST_P(AVCodecContextWrapperTest, TestAVCodecContextWrapper)
{
  const auto version = GetParam();
  RUN_TEST_FOR_VERSION(version, runAVCodecContextTest);
}

INSTANTIATE_TEST_SUITE_P(AVCodecContextWrapper,
                         AVCodecContextWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace ffmpeg::avcodec
