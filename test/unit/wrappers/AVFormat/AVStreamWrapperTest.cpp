/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVFormat/wrappers/AVStreamWrapper.h>
#include <common/InternalTypes.h>
#include <libHandling/FFmpegLibrariesMoc.h>
#include <wrappers/AVCodec/VersionToAVCodecTypes.h>
#include <wrappers/AVFormat/VersionToAVFormatTypes.h>
#include <wrappers/RunTestForAllVersions.h>
#include <wrappers/TestHelper.h>

#include <gtest/gtest.h>

#include <array>

namespace ffmpeg::avformat
{

namespace
{

using ffmpeg::internal::AVCodecContext;
using ffmpeg::internal::AVCodecDescriptor;
using ffmpeg::internal::AVCodecID;
using ffmpeg::internal::AVCodecParameters;
using ffmpeg::internal::AVCOL_SPC_FCC;
using ffmpeg::internal::AVMEDIA_TYPE_AUDIO;
using ffmpeg::internal::AVPixelFormat;
using ffmpeg::internal::AVRational;
using ffmpeg::internal::AVStream;
using ::testing::Return;

constexpr auto TEST_CODEC_ID = static_cast<AVCodecID>(234);

template <FFmpegVersion V> void runAVStreamWrapperTestDefaultValues()
{
  const auto version = getLibraryVerions(V);

  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  ffmpegLibraries->functionChecks.avcodecDescriptorGetExpectedID =
      ffmpeg::internal::AV_CODEC_ID_NONE;

  AVStreamType<V> stream;
  AVStreamWrapper streamWrapper(reinterpret_cast<AVStream *>(&stream), ffmpegLibraries);

  EXPECT_EQ(streamWrapper.getIndex(), 0);
  EXPECT_EQ(streamWrapper.getCodecType(), avutil::MediaType::Unknown);
  EXPECT_EQ(streamWrapper.getCodecID(), ffmpeg::internal::AV_CODEC_ID_NONE);
  EXPECT_EQ(streamWrapper.getAverageFrameRate(), Rational({0, 0}));
  EXPECT_EQ(streamWrapper.getTimeBase(), Rational({0, 0}));
  EXPECT_EQ(streamWrapper.getFrameSize(), Size());
  EXPECT_EQ(streamWrapper.getColorspace(), avutil::ColorSpace::UNSPECIFIED);
  EXPECT_EQ(streamWrapper.getPixelFormat().name, "Unknown");
  EXPECT_TRUE(streamWrapper.getExtradata().empty());
  EXPECT_FALSE(streamWrapper.getCodecParameters());

  EXPECT_FALSE(streamWrapper.getCodecDescriptor());
  EXPECT_EQ(ffmpegLibraries->functionCounters.avcodecDescriptorGet, 1);
}

template <FFmpegVersion V> void runAVStreamWrapperTestCodecContextSet()
{
  const auto version = getLibraryVerions(V);

  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  ffmpegLibraries->functionChecks.avcodecDescriptorGetExpectedID = TEST_CODEC_ID;

  AVStreamType<V> stream;
  stream.index          = 22;
  stream.time_base      = AVRational({12, 44});
  stream.avg_frame_rate = AVRational({13, 4});

  if constexpr (V == FFmpegVersion::FFmpeg_2x || V == FFmpegVersion::FFmpeg_3x ||
                V == FFmpegVersion::FFmpeg_4x)
  {
    avcodec::AVCodecContextType<V> codecContext;

    codecContext.codec_type = AVMEDIA_TYPE_AUDIO;
    codecContext.codec_id   = TEST_CODEC_ID;
    codecContext.time_base  = AVRational({7, 22});
    codecContext.width      = 640;
    codecContext.height     = 480;
    codecContext.colorspace = AVCOL_SPC_FCC;
    codecContext.pix_fmt    = static_cast<AVPixelFormat>(444);

    std::array<uint8_t, 3> TEST_EXTRADATA = {88, 99, 120};

    codecContext.extradata      = TEST_EXTRADATA.data();
    codecContext.extradata_size = static_cast<int>(TEST_EXTRADATA.size());

    stream.codec = reinterpret_cast<AVCodecContext *>(&codecContext);

    AVStreamWrapper streamWrapper(reinterpret_cast<AVStream *>(&stream), ffmpegLibraries);

    EXPECT_EQ(streamWrapper.getIndex(), 22);
    EXPECT_EQ(streamWrapper.getCodecType(), avutil::MediaType::Audio);
    EXPECT_EQ(streamWrapper.getCodecID(), TEST_CODEC_ID);
    EXPECT_EQ(streamWrapper.getAverageFrameRate(), Rational({13, 4}));
    EXPECT_EQ(streamWrapper.getTimeBase(), Rational({12, 44}));
    EXPECT_EQ(streamWrapper.getFrameSize(), Size({640, 480}));
    EXPECT_EQ(streamWrapper.getColorspace(), avutil::ColorSpace::FCC);
    EXPECT_EQ(streamWrapper.getExtradata(), dataArrayToByteVector(TEST_EXTRADATA));

    EXPECT_FALSE(streamWrapper.getCodecDescriptor());
    EXPECT_EQ(ffmpegLibraries->functionCounters.avcodecDescriptorGet, 1);

    EXPECT_FALSE(streamWrapper.getCodecParameters());
  }
  else
  {
    // The codec context in the stream was removed for newer versions
    AVStreamWrapper streamWrapper(reinterpret_cast<AVStream *>(&stream), ffmpegLibraries);

    EXPECT_EQ(streamWrapper.getIndex(), 22);
    EXPECT_EQ(streamWrapper.getAverageFrameRate(), Rational({13, 4}));
    EXPECT_EQ(streamWrapper.getTimeBase(), Rational({12, 44}));
  }
}

template <FFmpegVersion V> void runAVStreamWrapperTestCodecParametersSet()
{
  const auto version = getLibraryVerions(V);
  if constexpr (V == FFmpegVersion::FFmpeg_2x)
    return;

  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  ffmpegLibraries->functionChecks.avcodecDescriptorGetExpectedID = TEST_CODEC_ID;

  AVStreamType<V> stream;
  stream.index          = 22;
  stream.time_base      = AVRational({12, 44});
  stream.avg_frame_rate = AVRational({13, 4});

  AVCodecParametersType<V> codecParameter;

  codecParameter.codec_type  = AVMEDIA_TYPE_AUDIO;
  codecParameter.codec_id    = TEST_CODEC_ID;
  codecParameter.width       = 640;
  codecParameter.height      = 480;
  codecParameter.color_space = AVCOL_SPC_FCC;
  codecParameter.format      = 444;

  std::array<uint8_t, 3> TEST_EXTRADATA = {88, 99, 120};

  codecParameter.extradata      = TEST_EXTRADATA.data();
  codecParameter.extradata_size = static_cast<int>(TEST_EXTRADATA.size());

  if constexpr (V != FFmpegVersion::FFmpeg_2x)
    // Without this constexpr check, some compilers do not see that this code is never used and
    // AVStream_56 does not have codecpar.
    stream.codecpar = reinterpret_cast<AVCodecParameters *>(&codecParameter);

  AVStreamWrapper streamWrapper(reinterpret_cast<AVStream *>(&stream), ffmpegLibraries);

  EXPECT_EQ(streamWrapper.getIndex(), 22);
  EXPECT_EQ(streamWrapper.getCodecType(), avutil::MediaType::Audio);
  EXPECT_EQ(streamWrapper.getCodecID(), TEST_CODEC_ID);
  EXPECT_EQ(streamWrapper.getAverageFrameRate(), Rational({13, 4}));
  EXPECT_EQ(streamWrapper.getTimeBase(), Rational({12, 44}));
  EXPECT_EQ(streamWrapper.getFrameSize(), Size({640, 480}));
  EXPECT_EQ(streamWrapper.getColorspace(), avutil::ColorSpace::FCC);
  EXPECT_EQ(streamWrapper.getExtradata(), dataArrayToByteVector(TEST_EXTRADATA));

  EXPECT_FALSE(streamWrapper.getCodecDescriptor());
  EXPECT_EQ(ffmpegLibraries->functionCounters.avcodecDescriptorGet, 1);

  EXPECT_TRUE(streamWrapper.getCodecParameters());
}

} // namespace

class AVStreamWrapperTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST(AVStreamWrapperTest, shouldThrowIfLibraryNotSet)
{
  internal::avformat::AVStream_56   stream;
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries;

  EXPECT_THROW(AVStreamWrapper(reinterpret_cast<AVStream *>(&stream), ffmpegLibraries),
               std::runtime_error);
}

TEST(AVStreamWrapperTest, shouldThrowIfAVStreamPointerIsNull)
{
  AVStream *stream{};
  auto      ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();

  EXPECT_THROW(AVStreamWrapper(stream, ffmpegLibraries), std::runtime_error);
}

TEST_P(AVStreamWrapperTest, TestAVInputFormatWrapperDefaultValues)
{
  const auto version = GetParam();
  RUN_TEST_FOR_VERSION(version, runAVStreamWrapperTestDefaultValues);
}

TEST_P(AVStreamWrapperTest, TestAVInputFormatWrapperWithCodecContextSet)
{
  const auto version = GetParam();
  RUN_TEST_FOR_VERSION(version, runAVStreamWrapperTestCodecContextSet);
}

TEST_P(AVStreamWrapperTest, TestAVInputFormatWrapperWithCodecParametersSet)
{
  const auto version = GetParam();
  if (version.avformat.major == 56)
    // Version 56 does not have codec parameters yet
    return;

  RUN_TEST_FOR_VERSION(version, runAVStreamWrapperTestCodecParametersSet);
}

INSTANTIATE_TEST_SUITE_P(AVFormatWrappers,
                         AVStreamWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace ffmpeg::avformat
