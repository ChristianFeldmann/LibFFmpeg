/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <common/Error.h>
#include <common/InternalTypes.h>
#include <wrappers/AVCodec/AVPacketWrapper.h>
#include <wrappers/AVCodec/AVPacketWrapperInternal.h>
#include <wrappers/AVFormat/AVFormatContextWrapper.h>
#include <wrappers/AVFormat/AVFormatContextWrapperInternal.h>
#include <wrappers/AVFormat/AVInputFormatWrapperInternal.h>
#include <wrappers/AVFormat/AVStreamWrapperInternal.h>
#include <wrappers/TestHelper.h>

#include <libHandling/FFmpegLibrariesMoc.h>

#include <gtest/gtest.h>

namespace ffmpeg::avformat
{

namespace
{

using ffmpeg::internal::AVDictionary;
using ffmpeg::internal::AVFormatContext;
using ffmpeg::internal::AVInputFormat;
using ffmpeg::internal::AVPacket;
using ffmpeg::internal::AVStream;
using ffmpeg::internal::avcodec::AVPacket_56;
using ffmpeg::internal::avcodec::AVPacket_57;
using ffmpeg::internal::avcodec::AVPacket_58;
using ffmpeg::internal::avcodec::AVPacket_59;
using ffmpeg::internal::avcodec::AVPacket_60;
using ffmpeg::internal::avformat::AVFormatContext_56;
using ffmpeg::internal::avformat::AVFormatContext_57;
using ffmpeg::internal::avformat::AVFormatContext_58;
using ffmpeg::internal::avformat::AVFormatContext_59;
using ffmpeg::internal::avformat::AVFormatContext_60;
using ffmpeg::internal::avformat::AVInputFormat_56;
using ffmpeg::internal::avformat::AVInputFormat_57;
using ffmpeg::internal::avformat::AVInputFormat_58;
using ffmpeg::internal::avformat::AVInputFormat_59;
using ffmpeg::internal::avformat::AVInputFormat_60;
using ffmpeg::internal::avformat::AVStream_56;
using ffmpeg::internal::avformat::AVStream_57;
using ffmpeg::internal::avformat::AVStream_58;
using ffmpeg::internal::avformat::AVStream_59;
using ffmpeg::internal::avformat::AVStream_60;
using ::testing::Return;

constexpr auto TEST_START_TIME          = 483;
constexpr auto TEST_DURATION            = 852;
constexpr auto TEST_INPUT_FORMAT_NAME   = "TestName";
constexpr auto TEST_EXPECTED_NR_PACKETS = 5;

struct FFmpegCallbackCounters
{
  int formatOpenInputCount{};
  int formatCloseInputCount{};
  int findStreamInfoCount{};
  int readFrameCount{};
};
FFmpegCallbackCounters ffmpegCallbackCounters{};

void resetCallbackCounters()
{
  ffmpegCallbackCounters = {};
}

template <typename AVFormatContextType, typename AVInputFormatType>
int formatOpenInput(AVFormatContext **ps,
                    const char       *url,
                    AVInputFormat    *fmt,
                    AVDictionary    **options)
{
  EXPECT_EQ(std::string(url), "dummyFilePath");

  auto format        = new AVFormatContextType;
  format->nb_streams = 2;
  format->streams    = new AVStream *[format->nb_streams];
  *ps                = reinterpret_cast<AVFormatContext *>(format);

  auto inputFormat  = new AVInputFormatType;
  inputFormat->name = TEST_INPUT_FORMAT_NAME;
  format->iformat   = reinterpret_cast<AVInputFormat *>(inputFormat);

  format->start_time = TEST_START_TIME;
  format->duration   = TEST_DURATION;

  ffmpegCallbackCounters.formatOpenInputCount++;

  return toAVError(ReturnCode::Ok);
}

template <typename AVFormatContextType> void formatCloseInput(AVFormatContext **formatContext)
{
  auto castFormatContext = reinterpret_cast<AVFormatContextType *>(*formatContext);
  delete castFormatContext;
  formatContext = nullptr;

  ffmpegCallbackCounters.formatCloseInputCount++;
}

template <typename AVFormatContextType, typename AVStreamType>
int findStreamInfo(AVFormatContext *formatContext, AVDictionary **options)
{
  auto videoStream   = new AVStreamType;
  videoStream->index = 0;

  auto audioStream   = new AVStreamType;
  audioStream->index = 1;

  auto concreteFormatContext        = reinterpret_cast<AVFormatContextType *>(formatContext);
  concreteFormatContext->streams[0] = reinterpret_cast<AVStream *>(videoStream);
  concreteFormatContext->streams[1] = reinterpret_cast<AVStream *>(audioStream);

  ffmpegCallbackCounters.findStreamInfoCount++;

  return toAVError(ReturnCode::Ok);
}

template <typename AVPacketType> int readFrame(AVFormatContext *s, AVPacket *pkt)
{
  ffmpegCallbackCounters.readFrameCount++;

  if (ffmpegCallbackCounters.readFrameCount > TEST_EXPECTED_NR_PACKETS)
    return toAVError(ReturnCode::EndOfFile);

  auto packet = reinterpret_cast<AVPacketType *>(pkt);
  packet->pts = ffmpegCallbackCounters.readFrameCount * 100;

  return toAVError(ReturnCode::Ok);
}

template <typename AVPacketType> AVPacket *allocPacket()
{
  auto packet = new AVPacketType;
  packet->pts = 0;
  packet->dts = 0;
  return reinterpret_cast<AVPacket *>(packet);
}

template <typename AVPacketType> void freePacket(AVPacket **pkt)
{
  auto actualPacket = reinterpret_cast<AVPacketType *>(*pkt);
  delete actualPacket;
  pkt = nullptr;
}

template <typename AVFormatContextType,
          typename AVStreamType,
          typename AVInputFormatType,
          typename AVPacketType>
void runAVFormatContextWrapperTest(const LibraryVersions &version)
{
  resetCallbackCounters();

  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  ffmpegLibraries->avformat.avformat_open_input =
      formatOpenInput<AVFormatContextType, AVInputFormatType>;
  ffmpegLibraries->avformat.avformat_close_input = formatCloseInput<AVFormatContextType>;
  ffmpegLibraries->avformat.avformat_find_stream_info =
      findStreamInfo<AVFormatContextType, AVStreamType>;
  ffmpegLibraries->avformat.av_read_frame  = readFrame<AVPacketType>;
  ffmpegLibraries->avcodec.av_packet_alloc = allocPacket<AVPacketType>;
  ffmpegLibraries->avcodec.av_packet_free  = freePacket<AVPacketType>;

  {
    AVFormatContextWrapper format(ffmpegLibraries);
    const auto             result = format.openFile("dummyFilePath");
    EXPECT_TRUE(result.first);

    EXPECT_EQ(format.getStartTime(), TEST_START_TIME);
    EXPECT_EQ(format.getDuration(), TEST_DURATION);
    // Todo: Test metadata

    EXPECT_EQ(format.getNumberStreams(), 2);
    for (int i = 0; i < format.getNumberStreams(); ++i)
    {
      const auto stream = format.getStream(i);
      EXPECT_EQ(stream.getIndex(), i);
    }

    constexpr auto INVALID_STREAM_INDEX_NEGATIVE  = -8;
    constexpr auto INVALID_STREAM_INDEX_TOO_LARGE = 8;
    EXPECT_THROW(format.getStream(INVALID_STREAM_INDEX_NEGATIVE), std::runtime_error);
    EXPECT_THROW(format.getStream(INVALID_STREAM_INDEX_TOO_LARGE), std::runtime_error);

    int streamCounter = 0;
    for (const auto &stream : format.getStreams())
      EXPECT_EQ(stream.getIndex(), streamCounter++);

    const auto inputFormat = format.getInputFormat();
    EXPECT_EQ(inputFormat.getName(), TEST_INPUT_FORMAT_NAME);

    avcodec::AVPacketWrapper packet(ffmpegLibraries);
    for (int i = 0; i < TEST_EXPECTED_NR_PACKETS; ++i)
    {
      EXPECT_TRUE(format.getNextPacket(packet));
      EXPECT_EQ(packet.getPTS(), (i + 1) * 100);
    }
    EXPECT_FALSE(format.getNextPacket(packet));

    EXPECT_EQ(ffmpegCallbackCounters.formatOpenInputCount, 1);
    EXPECT_EQ(ffmpegCallbackCounters.findStreamInfoCount, 1);
    EXPECT_EQ(ffmpegCallbackCounters.readFrameCount, 6);
  }

  EXPECT_EQ(ffmpegCallbackCounters.formatCloseInputCount, 1);
}

} // namespace

class AVFormatContextWrapperTest : public testing::TestWithParam<LibraryVersions>
{
};

TEST_F(AVFormatContextWrapperTest, ConstructorWithNullptrShouldThrow)
{
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries;
  EXPECT_THROW(AVFormatContextWrapper format(ffmpegLibraries), std::runtime_error);
}

TEST_P(AVFormatContextWrapperTest, TestAVInputFormatWrapperDefaultValues)
{
  const auto version = GetParam();
  if (version.avformat.major == 56)
    runAVFormatContextWrapperTest<AVFormatContext_56, AVStream_56, AVInputFormat_56, AVPacket_56>(
        version);
  else if (version.avformat.major == 57)
    runAVFormatContextWrapperTest<AVFormatContext_57, AVStream_57, AVInputFormat_57, AVPacket_57>(
        version);
  else if (version.avformat.major == 58)
    runAVFormatContextWrapperTest<AVFormatContext_58, AVStream_58, AVInputFormat_58, AVPacket_58>(
        version);
  else if (version.avformat.major == 59)
    runAVFormatContextWrapperTest<AVFormatContext_59, AVStream_59, AVInputFormat_59, AVPacket_59>(
        version);
  else if (version.avformat.major == 60)
    runAVFormatContextWrapperTest<AVFormatContext_60, AVStream_60, AVInputFormat_60, AVPacket_60>(
        version);
}

INSTANTIATE_TEST_SUITE_P(AVFormatWrappers,
                         AVFormatContextWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace ffmpeg::avformat
