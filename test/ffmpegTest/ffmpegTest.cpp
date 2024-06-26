/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <Decoder.h>
#include <Demuxer.h>
#include <common/ComparisonFunctions.h>
#include <common/Error.h>
#include <common/Formatting.h>
#include <common/Functions.h>
#include <libHandling/FFmpegLibrariesBuilder.h>
#include <libHandling/libraryFunctions/Functions.h>

#include <array>
#include <functional>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace libffmpeg;

using libffmpeg::avutil::ColorSpace;
using libffmpeg::avutil::MediaType;
using libffmpeg::avutil::PictureType;
using ::testing::Contains;

namespace
{

constexpr std::size_t operator"" _sz(unsigned long long n)
{
  return n;
}

// Generated with
// ffmpeg -y -f lavfi -i testsrc -f lavfi -i "sine=frequency=1000:duration=60" -f lavfi -i
// "sine=frequency=600:duration=60" -filter_complex amerge -t 1 -pix_fmt yuv420p
// TestFile_h264_aac_1s_320x240.mp4
constexpr auto TEST_FILE_NAME = "TestFile_h264_aac_1s_320x240.mp4";

class LibrariesWithLogging
{
public:
  LibrariesWithLogging()
  {
    const auto loadingResult =
        FFmpegLibrariesBuilder()
            .withAdditionalSearchPaths({".", ""})
            .withLoggingFunction([this](const LogLevel logLevel, const std::string &message)
                                 { this->loggingFunction(logLevel, message); },
                                 LogLevel::Debug)
            .tryLoadingOfLibraries();
    EXPECT_TRUE(loadingResult) << "Error loading libraries";
    this->libraries = loadingResult.ffmpegLibraries;
  }

  Demuxer openTestFileInDemuxer()
  {
    Demuxer demuxer(this->libraries);
    EXPECT_TRUE(demuxer.openFile(TEST_FILE_NAME))
        << "Opening test file " << TEST_FILE_NAME << " failed.";

    return demuxer;
  }

  void loggingFunction(const LogLevel logLevel, const std::string &message)
  {
    if (logLevel != LogLevel::Debug)
      std::cerr << "[          ] [" << to_string(logLevel) << "] " << message << "\n";
    this->logEntries.push_back({logLevel, message});
  }

  bool containsLogEntry(const LogEntry &logEntry)
  {
    return std::find(this->logEntries.begin(), this->logEntries.end(), logEntry) !=
           this->logEntries.end();
  }

  std::vector<LogEntry>             logEntries{};
  std::shared_ptr<IFFmpegLibraries> libraries;
};

} // namespace

TEST(FFmpegTest, LoadLibrariesAndLogVersion)
{
  auto libsAndLogs = LibrariesWithLogging();

  const auto librariesInfo = libsAndLogs.libraries->getLibrariesInfo();
  EXPECT_EQ(librariesInfo.size(), 4);

  for (const auto &libInfo : librariesInfo)
    std::cerr << "[          ] [INFO] Lib " << libInfo.name << " loaded from path " << libInfo.path
              << " with version " << libInfo.version << "\n";
}

TEST(FFmpegTest, LoadLibrariesAndCheckLoggingCallback)
{
  auto libsAndLogs = LibrariesWithLogging();

  EXPECT_FALSE(libsAndLogs.logEntries.empty());
  EXPECT_TRUE(libsAndLogs.containsLogEntry({LogLevel::Info, "Logging function set successfully"}));
  EXPECT_TRUE(libsAndLogs.containsLogEntry(
      {LogLevel::Debug, "Successfully resolved function avutil_version"}));
  EXPECT_TRUE(libsAndLogs.containsLogEntry({LogLevel::Info, "Setting up av logging callback"}));
  EXPECT_TRUE(libsAndLogs.containsLogEntry(
      {LogLevel::Info,
       "Loading of ffmpeg libraries successfully finished. FFmpeg is ready to be used."}));

  EXPECT_FALSE(
      libsAndLogs.containsLogEntry({LogLevel::Info, "Disconnectiong av logging callback"}));

  libsAndLogs.libraries.reset();

  EXPECT_TRUE(libsAndLogs.containsLogEntry({LogLevel::Info, "Disconnectiong av logging callback"}));
}

TEST(FFmpegTest, CheckFormatAndStreamParameters)
{
  auto libsAndLogs = LibrariesWithLogging();

  auto       demuxer       = libsAndLogs.openTestFileInDemuxer();
  const auto formatContext = demuxer.getFormatContext();
  const auto majorVersion  = libsAndLogs.libraries->getLibrariesVersion().avformat.major;

  if (majorVersion <= 56)
    EXPECT_EQ(formatContext->getStartTime(), -23220);
  else
    EXPECT_EQ(formatContext->getStartTime(), 0);

  EXPECT_EQ(formatContext->getDuration(), 1000000);
  EXPECT_EQ(formatContext->getNumberStreams(), 2);

  const auto metadataMap = formatContext->getMetadata().toMap();
  EXPECT_EQ(metadataMap.size(), 4);
  EXPECT_EQ(metadataMap.count("compatible_brands"), 1);
  EXPECT_EQ(metadataMap.at("compatible_brands"), "isomiso2avc1mp41");
  EXPECT_EQ(metadataMap.count("encoder"), 1);
  EXPECT_EQ(metadataMap.at("encoder"), "Lavf59.16.100");
  EXPECT_EQ(metadataMap.count("major_brand"), 1);
  EXPECT_EQ(metadataMap.at("major_brand"), "isom");
  EXPECT_EQ(metadataMap.count("minor_version"), 1);
  EXPECT_EQ(metadataMap.at("minor_version"), "512");

  const auto inputFormat = formatContext->getInputFormat();
  EXPECT_EQ(inputFormat.getName(), "mov,mp4,m4a,3gp,3g2,mj2");
  EXPECT_EQ(inputFormat.getLongName(), "QuickTime / MOV");
  EXPECT_THAT((std::array{"mov,mp4,m4a,3gp,3g2,mj2",
                          "mov,mp4,m4a,3gp,3g2,mj2,psp,m4b,ism,ismv,isma,f4v",
                          "mov,mp4,m4a,3gp,3g2,mj2,psp,m4b,ism,ismv,isma,f4v,avif",
                          "mov,mp4,m4a,3gp,3g2,mj2,psp,m4b,ism,ismv,isma,f4v,avif,heic,heif"}),
              Contains(inputFormat.getExtensions()));

  avformat::AVInputFormatFlags expectedFlags{};
  if (majorVersion > 58)
    // Older (<= ffmpeg 4) versions will report this flag as false
    expectedFlags.showIDs = true;
  expectedFlags.noByteSeek = true;
  if (majorVersion > 57)
    // Older (<= ffmpeg 2) versions will report this flag as false
    expectedFlags.seekToPTS = true;

  EXPECT_EQ(inputFormat.getMimeType(), "");
  EXPECT_EQ(inputFormat.getFlags(), expectedFlags);

  const auto &audioStream = formatContext->getStream(0);
  EXPECT_EQ(audioStream.getCodecType(), MediaType::Audio);

  const auto audioCodecDescriptor = audioStream.getCodecDescriptor();
  EXPECT_EQ(audioCodecDescriptor->mediaType, MediaType::Audio);
  EXPECT_EQ(audioCodecDescriptor->codecName, "aac");
  EXPECT_EQ(audioCodecDescriptor->longName, "AAC (Advanced Audio Coding)");
  avcodec::CodecDescriptorProperties expectedAudioProperties{};
  if (majorVersion >= 58)
    // Older (< ffmpeg 4) versions will report this flag as false
    expectedAudioProperties.intraOnly = true;
  expectedAudioProperties.lossy = true;
  EXPECT_EQ(audioCodecDescriptor->properties, expectedAudioProperties);
  EXPECT_EQ(audioCodecDescriptor->mimeTypes.size(), 0);

  if (majorVersion == 56)
    // FFmpeg versions 2 does not parse profiles in the descriptor
    EXPECT_EQ(audioCodecDescriptor->profiles.size(), 0);
  else if (majorVersion == 57)
    // For some reason, the "LC" profile is reported twice for FFmpeg 3
    EXPECT_TRUE(areEqual(audioCodecDescriptor->profiles,
                         {"LC", "HE-AAC", "HE-AACv2", "LD", "ELD", "Main", "LC", "SSR", "LTP"}));
  else
    EXPECT_TRUE(areEqual(audioCodecDescriptor->profiles,
                         {"LC", "HE-AAC", "HE-AACv2", "LD", "ELD", "Main", "SSR", "LTP"}));

  EXPECT_EQ(audioStream.getAverageFrameRate(), Rational({24, 1}));
  EXPECT_EQ(audioStream.getTimeBase(), Rational({1, 44100}));
  EXPECT_EQ(audioStream.getFrameSize(), Size({0, 0}));
  EXPECT_EQ(audioStream.getColorspace(), ColorSpace::UNSPECIFIED);
  EXPECT_EQ(audioStream.getExtradata().size(), 5);
  EXPECT_EQ(audioStream.getIndex(), 0);

  const auto &videoStream = formatContext->getStream(1);
  EXPECT_EQ(videoStream.getCodecType(), MediaType::Video);

  const auto videoCodecDescriptor = videoStream.getCodecDescriptor();
  EXPECT_EQ(videoCodecDescriptor->mediaType, MediaType::Video);
  EXPECT_EQ(videoCodecDescriptor->codecName, "h264");
  EXPECT_EQ(videoCodecDescriptor->longName, "H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10");
  avcodec::CodecDescriptorProperties expectedVideoProperties{};
  expectedVideoProperties.lossy    = true;
  expectedVideoProperties.lossless = true;
  expectedVideoProperties.reorder  = true;
  EXPECT_EQ(videoCodecDescriptor->properties, expectedVideoProperties);
  EXPECT_EQ(videoCodecDescriptor->mimeTypes.size(), 0);

  if (majorVersion > 56)
    EXPECT_TRUE(areEqual(videoCodecDescriptor->profiles,
                         {"Baseline",
                          "Constrained Baseline",
                          "Main",
                          "Extended",
                          "High",
                          "High 10",
                          "High 10 Intra",
                          "High 4:2:2",
                          "High 4:2:2 Intra",
                          "High 4:4:4",
                          "High 4:4:4 Predictive",
                          "High 4:4:4 Intra",
                          "CAVLC 4:4:4",
                          "Multiview High",
                          "Stereo High"}));
  else
    // Old ffmpeg versions do not parse profiles in the descriptor
    EXPECT_EQ(videoCodecDescriptor->profiles.size(), 0);

  EXPECT_EQ(videoStream.getAverageFrameRate(), Rational({25, 1}));
  EXPECT_EQ(videoStream.getTimeBase(), Rational({1, 12800}));
  EXPECT_EQ(videoStream.getFrameSize(), Size({320, 240}));
  EXPECT_EQ(videoStream.getColorspace(), ColorSpace::UNSPECIFIED);
  EXPECT_EQ(videoStream.getPixelFormat().name, "yuv420p");
  EXPECT_EQ(videoStream.getExtradata().size(), 46);
  EXPECT_EQ(videoStream.getIndex(), 1);
}

TEST(FFmpegTest, DemuxPackets)
{
  auto libsAndLogs = LibrariesWithLogging();

  auto       demuxer       = libsAndLogs.openTestFileInDemuxer();
  const auto formatContext = demuxer.getFormatContext();

  constexpr std::array<int, 25> expectedDataSizesVideo = {3827, 499, 90,  46, 29,  439, 82, 36, 40,
                                                          461,  56,  36,  25, 419, 54,  29, 23, 339,
                                                          50,   46,  287, 54, 30,  171, 141};
  constexpr std::array<int, 45> expectedDataSizesAudio = {
      343, 377, 258, 299, 304, 339, 316, 362, 353, 358, 385, 358, 348, 365, 358,
      364, 369, 385, 339, 377, 348, 368, 370, 367, 353, 348, 397, 391, 410, 337,
      372, 370, 372, 342, 431, 378, 370, 333, 355, 366, 355, 388, 412, 349, 7};

  int packetCountAudio = 0;
  int packetCountVideo = 0;
  while (auto packet = demuxer.getNextPacket())
  {
    EXPECT_TRUE(packet->getStreamIndex() == 0 || packet->getStreamIndex() == 1);
    if (packet->getStreamIndex() == 0)
    {
      if (packetCountAudio == 0 && libsAndLogs.libraries->getLibrariesVersion().avcodec.major == 57)
        // For some reason, the first packet is 23 bytes larger for this version of FFmpeg.
        EXPECT_EQ(packet->getDataSize(), 366) << "Audio packet number " << packetCountAudio;
      else
        EXPECT_EQ(packet->getDataSize(), expectedDataSizesAudio.at(packetCountAudio))
            << "Audio packet number " << packetCountAudio;
      ++packetCountAudio;
    }
    else if (packet->getStreamIndex() == 1)
    {
      EXPECT_EQ(packet->getDataSize(), expectedDataSizesVideo.at(packetCountVideo))
          << "Video packet number " << packetCountVideo;
      ++packetCountVideo;
    }
  }

  EXPECT_EQ(packetCountAudio, 45);
  EXPECT_EQ(packetCountVideo, 25);
}

TEST(FFmpegTest, DemuxPacketsAndCheckFFmpegCallbackLogs)
{
  auto libsAndLogs = LibrariesWithLogging();

  auto       demuxer       = libsAndLogs.openTestFileInDemuxer();
  const auto formatContext = demuxer.getFormatContext();

  while (auto packet = demuxer.getNextPacket())
  {
    EXPECT_TRUE(packet->getStreamIndex() == 0 || packet->getStreamIndex() == 1);
  }

  const auto majorVersion = libsAndLogs.libraries->getLibrariesVersion().avutil.major;

  EXPECT_TRUE(libsAndLogs.containsLogEntry(
      {LogLevel::Debug, "Got Packet with  DTS -1024 PTS 0 Flags [Keyframe]"}));
  if (majorVersion == 55)
    EXPECT_TRUE(
        libsAndLogs.containsLogEntry({LogLevel::Debug, "nal_unit_type: 8, nal_ref_idc: 3\n"}));
  else if (majorVersion > 55)
    EXPECT_TRUE(
        libsAndLogs.containsLogEntry({LogLevel::Debug, "nal_unit_type: 8(PPS), nal_ref_idc: 3\n"}));
  EXPECT_TRUE(libsAndLogs.containsLogEntry({LogLevel::Debug, "stream 0, sample 0, dts -23220\n"}));
}

TEST(FFmpegTest, DecodingTest)
{
  auto libsAndLogs = LibrariesWithLogging();

  auto demuxer = libsAndLogs.openTestFileInDemuxer();
  auto decoder = Decoder(libsAndLogs.libraries);

  const auto streamToDecode      = 1;
  const auto stream              = demuxer.getFormatContext()->getStream(streamToDecode);
  const auto avcodecVersionMajor = libsAndLogs.libraries->getLibrariesVersion().avcodec.major;

  // The amount of padding that FFmpeg uses depends on how it was compiled.
  // Here, we use our own compiled versions. But in general this can not be predicted.
  constexpr std::array<int, 2> EXPECTED_LINESIZE_LUMA   = {320, 384};
  constexpr std::array<int, 2> EXPECTED_LINESIZE_CHROMA = {160, 192};

  ASSERT_TRUE(decoder.openForDecoding(stream));

  auto totalFrameCounter = 0;

  auto pullFramesFromDecoder = [&decoder,
                                &totalFrameCounter,
                                &EXPECTED_LINESIZE_LUMA,
                                &EXPECTED_LINESIZE_CHROMA,
                                &avcodecVersionMajor]()
  {
    int framesDecodedInLoop = 0;
    while (const auto frame = decoder.decodeNextFrame())
    {
      EXPECT_TRUE(frame);

      EXPECT_EQ(frame->getPixelFormatDescriptor().name, "yuv420p");

      EXPECT_EQ(frame->getSize(), Size({320, 240}));
      EXPECT_THAT(EXPECTED_LINESIZE_LUMA, Contains(frame->getLineSize(0)));
      EXPECT_THAT(EXPECTED_LINESIZE_CHROMA, Contains(frame->getLineSize(1)));
      EXPECT_THAT(EXPECTED_LINESIZE_CHROMA, Contains(frame->getLineSize(2)));
      EXPECT_EQ(frame->getSampleAspectRatio(), Rational({1, 1}));

      const auto absoluteFrameIndex = totalFrameCounter + framesDecodedInLoop;

      constexpr std::array<PictureType, 25> expectedPictureTypes = {
          PictureType::I, PictureType::B, PictureType::B, PictureType::B, PictureType::P,
          PictureType::B, PictureType::B, PictureType::B, PictureType::P, PictureType::B,
          PictureType::B, PictureType::B, PictureType::P, PictureType::B, PictureType::B,
          PictureType::B, PictureType::P, PictureType::B, PictureType::B, PictureType::P,
          PictureType::B, PictureType::B, PictureType::P, PictureType::P, PictureType::P};
      EXPECT_EQ(frame->getPictType(), expectedPictureTypes.at(absoluteFrameIndex));

      constexpr std::array<int64_t, 25> expectedPTSValues = {
          0,    512,  1024, 1536, 2048, 2560, 3072, 3584,  4096,  4608,  5120,  5632, 6144,
          6656, 7168, 7680, 8192, 8704, 9216, 9728, 10240, 10752, 11264, 11776, 12288};
      if (avcodecVersionMajor == 56)
        EXPECT_FALSE(frame->getPTS());
      else
        EXPECT_EQ(frame->getPTS(), expectedPTSValues.at(absoluteFrameIndex));

      constexpr std::array<std::int64_t, 25> expectedFrameHashes = {
          -3007600926112186533, -3391200051889142087, -1160633604618185881, 4273627566792723166,
          -4154039706688151037, -1301428058581480615, 7452780297656341798,  7600256964838247071,
          -8499296325100989937, -8234365708930177171, -6091801175307918277, -6243952166556384038,
          -1538307937175716804, -1214069488220656158, 221111691894747256,   -6002299333845183392,
          2227144204187938134,  6419852838956731569,  -2032560420377261054, 84234931995669811,
          -6767048039990945080, -8993409000448418032, -7272467650392102601, 3844105725739243304,
          -2350406183677959349};
      EXPECT_EQ(calculateFrameDataHash(*frame), expectedFrameHashes.at(absoluteFrameIndex));

      ++framesDecodedInLoop;
    }
    return framesDecodedInLoop;
  };

  while (const auto packet = demuxer.getNextPacket())
  {
    if (packet->getStreamIndex() != streamToDecode)
      continue;

    EXPECT_EQ(decoder.getDecoderState(), Decoder::State::NeedsMoreData);

    auto result = decoder.sendPacket(*packet);
    ASSERT_NE(result, Decoder::SendPacketResult::Error);

    if (decoder.getDecoderState() == Decoder::State::RetrieveFrames)
    {
      const auto framesDecoded = pullFramesFromDecoder();
      EXPECT_GT(framesDecoded, 0);
      totalFrameCounter += framesDecoded;
      EXPECT_EQ(decoder.getDecoderState(), Decoder::State::NeedsMoreData);
    }

    if (result == Decoder::SendPacketResult::NotSentPullFramesFirst)
    {
      result = decoder.sendPacket(*packet);
      EXPECT_EQ(result, Decoder::SendPacketResult::Ok);
    }
  }

  decoder.setFlushing();

  EXPECT_EQ(decoder.getDecoderState(), Decoder::State::RetrieveFrames);
  const auto framesDecoded = pullFramesFromDecoder();
  EXPECT_GT(framesDecoded, 0);
  totalFrameCounter += framesDecoded;
  EXPECT_EQ(decoder.getDecoderState(), Decoder::State::EndOfBitstream);

  EXPECT_EQ(totalFrameCounter, 25);
}
