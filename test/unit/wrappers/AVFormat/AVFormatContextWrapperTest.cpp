/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVCodec/wrappers/AVPacketWrapper.h>
#include <AVCodec/wrappers/AVPacketWrapperInternal.h>
#include <AVFormat/wrappers/AVFormatContextWrapper.h>
#include <AVFormat/wrappers/AVFormatContextWrapperInternal.h>
#include <AVFormat/wrappers/AVInputFormatWrapperInternal.h>
#include <AVFormat/wrappers/AVStreamWrapperInternal.h>
#include <common/Error.h>
#include <common/InternalTypes.h>
#include <wrappers/TestHelper.h>

#include <libHandling/FFmpegLibrariesMoc.h>

#include <gtest/gtest.h>

namespace ffmpeg::avformat
{

namespace
{

using ffmpeg::internal::AVDictionary;
using ffmpeg::internal::AVDictionaryEntry;
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

template <typename AVFormatContextType,
          typename AVStreamType,
          typename AVInputFormatType,
          typename AVPacketType>
void runAVFormatContextWrapperTest(const LibraryVersions &version)
{
  std::array<AVDictionaryEntry, 5> TEST_METADATA_ENTRIES = {
      AVDictionaryEntry({(char *)"key1", (char *)"value1"}),
      AVDictionaryEntry({(char *)"key2", (char *)"value2"}),
      AVDictionaryEntry({nullptr, nullptr})};
  avutil::DictionaryMap TEST_METADATA_REFERENCE_ENTRIES = {{"key1", "value1"}, {"key2", "value2"}};

  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(version));

  int formatOpenInputCount = 0;
  ffmpegLibraries->avformat.avformat_open_input =
      [&](AVFormatContext **ps, const char *url, AVInputFormat *fmt, AVDictionary **options)
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
    format->metadata   = reinterpret_cast<AVDictionary *>(TEST_METADATA_ENTRIES.data());

    formatOpenInputCount++;

    return toAVError(ReturnCode::Ok);
  };

  int formatCloseInputCount = 0;
  int streamsFreed          = 0;
  ffmpegLibraries->avformat.avformat_close_input =
      [&formatCloseInputCount, &streamsFreed](AVFormatContext **formatContext)
  {
    auto castFormatContext = reinterpret_cast<AVFormatContextType *>(*formatContext);

    if (castFormatContext->streams)
    {
      for (int i = 0; i < castFormatContext->nb_streams; ++i)
      {
        auto castStream = reinterpret_cast<AVStreamType *>(castFormatContext->streams[i]);
        if (castStream)
        {
          ++streamsFreed;
          delete castStream;
        }
      }

      delete[] castFormatContext->streams;
      castFormatContext->streams = nullptr;
    }

    if (castFormatContext->iformat)
    {
      auto castInputFormatType = reinterpret_cast<AVInputFormatType *>(castFormatContext->iformat);
      delete castInputFormatType;
      castFormatContext->iformat = nullptr;
    }

    delete castFormatContext;
    formatContext = nullptr;

    formatCloseInputCount++;
  };

  int findStreamInfoCount = 0;
  int streamsAllocated    = 0;
  ffmpegLibraries->avformat.avformat_find_stream_info =
      [&findStreamInfoCount, &streamsAllocated](AVFormatContext *formatContext,
                                                AVDictionary   **options)
  {
    auto videoStream   = new AVStreamType;
    videoStream->index = 0;

    auto audioStream   = new AVStreamType;
    audioStream->index = 1;

    auto concreteFormatContext        = reinterpret_cast<AVFormatContextType *>(formatContext);
    concreteFormatContext->streams[0] = reinterpret_cast<AVStream *>(videoStream);
    concreteFormatContext->streams[1] = reinterpret_cast<AVStream *>(audioStream);
    concreteFormatContext->nb_streams = 2;

    streamsAllocated += 2;
    findStreamInfoCount++;

    return toAVError(ReturnCode::Ok);
  };

  int readFrameCount                      = 0;
  ffmpegLibraries->avformat.av_read_frame = [&readFrameCount](AVFormatContext *s, AVPacket *pkt)
  {
    readFrameCount++;

    if (readFrameCount > TEST_EXPECTED_NR_PACKETS)
      return toAVError(ReturnCode::EndOfFile);

    auto packet = reinterpret_cast<AVPacketType *>(pkt);
    packet->pts = readFrameCount * 100;

    return toAVError(ReturnCode::Ok);
  };

  ffmpegLibraries->avcodec.av_packet_alloc = []()
  {
    auto packet = new AVPacketType;
    packet->pts = 0;
    packet->dts = 0;
    return reinterpret_cast<AVPacket *>(packet);
  };

  ffmpegLibraries->avcodec.av_packet_free = [](AVPacket **pkt)
  {
    auto actualPacket = reinterpret_cast<AVPacketType *>(*pkt);
    delete actualPacket;
    pkt = nullptr;
  };

  {
    AVFormatContextWrapper format(ffmpegLibraries);
    auto                   result = format.openFile("dummyFilePath");
    EXPECT_TRUE(result.first);

    result = format.openFile("anotherDummyFile");
    EXPECT_FALSE(result.first);

    EXPECT_EQ(format.getStartTime(), TEST_START_TIME);
    EXPECT_EQ(format.getDuration(), TEST_DURATION);

    EXPECT_EQ(format.getMetadata().toMap(), TEST_METADATA_REFERENCE_ENTRIES);
    EXPECT_EQ(ffmpegLibraries->functionCounters.avDictGet, 3);

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

    EXPECT_EQ(formatOpenInputCount, 1);
    EXPECT_EQ(findStreamInfoCount, 1);
    EXPECT_EQ(readFrameCount, 6);
  }

  EXPECT_EQ(formatCloseInputCount, 1);
  EXPECT_EQ(streamsAllocated, streamsFreed);
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
