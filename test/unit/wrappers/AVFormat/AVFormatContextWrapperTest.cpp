/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVCodec/wrappers/AVPacketWrapper.h>
#include <AVFormat/wrappers/AVFormatContextWrapper.h>
#include <common/Error.h>
#include <common/InternalTypes.h>
#include <libHandling/FFmpegLibrariesMoc.h>
#include <wrappers/AVCodec/VersionToAVCodecTypes.h>
#include <wrappers/AVFormat/VersionToAVFormatTypes.h>
#include <wrappers/RunTestForAllVersions.h>
#include <wrappers/TestHelper.h>

#include <gtest/gtest.h>

namespace libffmpeg::avformat
{

namespace
{

using libffmpeg::avcodec::AVPacketType;
using libffmpeg::internal::AVDictionary;
using libffmpeg::internal::AVDictionaryEntry;
using libffmpeg::internal::AVFormatContext;
using libffmpeg::internal::AVInputFormat;
using libffmpeg::internal::AVPacket;
using libffmpeg::internal::AVStream;
using ::testing::Return;

constexpr auto TEST_START_TIME          = 483;
constexpr auto TEST_DURATION            = 852;
constexpr auto TEST_INPUT_FORMAT_NAME   = "TestName";
constexpr auto TEST_EXPECTED_NR_PACKETS = 5;

template <FFmpegVersion V> void runAVFormatContextWrapperTest()
{
  std::array<AVDictionaryEntry, 5> TEST_METADATA_ENTRIES = {
      AVDictionaryEntry({(char *)"key1", (char *)"value1"}),
      AVDictionaryEntry({(char *)"key2", (char *)"value2"}),
      AVDictionaryEntry({nullptr, nullptr})};
  avutil::DictionaryMap TEST_METADATA_REFERENCE_ENTRIES = {{"key1", "value1"}, {"key2", "value2"}};

  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();
  EXPECT_CALL(*ffmpegLibraries, getLibrariesVersion()).WillRepeatedly(Return(getLibraryVerions(V)));

  int formatOpenInputCount = 0;
  ffmpegLibraries->avformat.avformat_open_input =
      [&](AVFormatContext **ps, const char *url, AVInputFormat *fmt, AVDictionary **options) {
        EXPECT_EQ(std::string(url), "dummyFilePath");

        auto format        = new AVFormatContextType<V>;
        format->nb_streams = 2;
        format->streams    = new AVStream *[format->nb_streams];
        *ps                = reinterpret_cast<AVFormatContext *>(format);

        auto inputFormat  = new AVInputFormatType<V>;
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
      [&formatCloseInputCount, &streamsFreed](AVFormatContext **formatContext) {
        auto castFormatContext = reinterpret_cast<AVFormatContextType<V> *>(*formatContext);

        if (castFormatContext->streams)
        {
          for (int i = 0; i < static_cast<int>(castFormatContext->nb_streams); ++i)
          {
            auto castStream = reinterpret_cast<AVStreamType<V> *>(castFormatContext->streams[i]);
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
          auto castInputFormatType =
              reinterpret_cast<AVInputFormatType<V> *>(castFormatContext->iformat);
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
                                                AVDictionary **  options) {
        auto videoStream   = new AVStreamType<V>;
        videoStream->index = 0;

        auto audioStream   = new AVStreamType<V>;
        audioStream->index = 1;

        auto concreteFormatContext = reinterpret_cast<AVFormatContextType<V> *>(formatContext);
        concreteFormatContext->streams[0] = reinterpret_cast<AVStream *>(videoStream);
        concreteFormatContext->streams[1] = reinterpret_cast<AVStream *>(audioStream);
        concreteFormatContext->nb_streams = 2;

        streamsAllocated += 2;
        findStreamInfoCount++;

        return toAVError(ReturnCode::Ok);
      };

  int readFrameCount                      = 0;
  ffmpegLibraries->avformat.av_read_frame = [&readFrameCount](AVFormatContext *s, AVPacket *pkt) {
    readFrameCount++;

    if (readFrameCount > TEST_EXPECTED_NR_PACKETS)
      return toAVError(ReturnCode::EndOfFile);

    auto packet = reinterpret_cast<AVPacketType<V> *>(pkt);
    packet->pts = readFrameCount * 100;

    return toAVError(ReturnCode::Ok);
  };

  ffmpegLibraries->avcodec.av_packet_alloc = []() {
    auto packet = new AVPacketType<V>;
    packet->pts = 0;
    packet->dts = 0;
    return reinterpret_cast<AVPacket *>(packet);
  };

  ffmpegLibraries->avcodec.av_packet_free = [](AVPacket **pkt) {
    auto actualPacket = reinterpret_cast<AVPacketType<V> *>(*pkt);
    delete actualPacket;
    pkt = nullptr;
  };

  {
    AVFormatContextWrapper format(ffmpegLibraries);
    EXPECT_TRUE(format.openFile("dummyFilePath"));
    EXPECT_FALSE(format.openFile("anotherDummyFile"));

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
  RUN_TEST_FOR_VERSION(version, runAVFormatContextWrapperTest);
}

INSTANTIATE_TEST_SUITE_P(AVFormatWrappers,
                         AVFormatContextWrapperTest,
                         testing::ValuesIn(SupportedFFmpegVersions),
                         getNameWithFFmpegVersion);

} // namespace libffmpeg::avformat
