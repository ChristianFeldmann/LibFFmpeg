/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <Decoder.h>
#include <Demuxer.h>
#include <common/ComparisonFunctions.h>
#include <common/Error.h>
#include <common/Functions.h>
#include <libHandling/FFmpegLibrariesBuilder.h>
#include <libHandling/libraryFunctions/Functions.h>

#include <array>
#include <functional>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ffmpeg;

using ffmpeg::avutil::ColorSpace;
using ffmpeg::avutil::MediaType;
using ffmpeg::avutil::PictureType;

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

std::shared_ptr<IFFmpegLibraries> openLibraries()
{
  const auto loadingResult =
      FFmpegLibrariesBuilder().withAdditionalSearchPaths({"."}).tryLoadingOfLibraries();
  EXPECT_TRUE(loadingResult) << "Error loading libraries";
  return loadingResult.ffmpegLibraries;
}

Demuxer openTestFileInDemuxer(std::shared_ptr<IFFmpegLibraries> ffmpegLibraries)
{
  Demuxer demuxer(ffmpegLibraries);
  const auto [openSuccessfull, openingLog] = demuxer.openFile(TEST_FILE_NAME);
  EXPECT_TRUE(openSuccessfull) << "Opening test file " << TEST_FILE_NAME << " failed.";

  return demuxer;
}

} // namespace

TEST(FFmpegTest, LoadLibrariesAndLogVersion)
{
  const auto loadingResult =
      FFmpegLibrariesBuilder().withAdditionalSearchPaths({"."}).tryLoadingOfLibraries();

  ASSERT_TRUE(loadingResult) << "Error loading ffmpeg library: " << loadingResult.errorMessage
                             << "\nLog:\n"
                             << to_string(loadingResult.loadingLog, ConcatenationSymbol::Newline);

  const auto librariesInfo = loadingResult.ffmpegLibraries->getLibrariesInfo();
  EXPECT_EQ(librariesInfo.size(), 4);

  for (const auto &libInfo : librariesInfo)
    std::cerr << "[          ] [INFO] Lib " << libInfo.name << " loaded from path " << libInfo.path
              << " with version " << libInfo.version << "\n";
}

TEST(FFmpegTest, CheckFormatAndStreamParameters)
{
  auto       ffmpegLibraries = openLibraries();
  auto       demuxer         = openTestFileInDemuxer(ffmpegLibraries);
  const auto formatContext   = demuxer.getFormatContext();
  const auto majorVersion    = ffmpegLibraries->getLibrariesVersion().avformat.major;

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
                          "mov,mp4,m4a,3gp,3g2,mj2,psp,m4b,ism,ismv,isma,f4v,avif"}),
              testing::Contains(inputFormat.getExtensions()));

  avformat::AVInputFormatFlags expectedFlags{};
  if (majorVersion > 58)
    // Older (<= ffmpeg 4) versions will report this flag as false
    expectedFlags.showIDs = true;
  expectedFlags.noByteSeek = true;
  if (majorVersion > 56)
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
  if (majorVersion > 58)
    // Older (<= ffmpeg 4) versions will report this flag as false
    expectedAudioProperties.intraOnly = true;
  expectedAudioProperties.lossy = true;
  EXPECT_EQ(audioCodecDescriptor->properties, expectedAudioProperties);
  EXPECT_EQ(audioCodecDescriptor->mimeTypes.size(), 0);

  if (majorVersion > 56)
    EXPECT_TRUE(areEqual(audioCodecDescriptor->profiles,
                         {"LC", "HE-AAC", "HE-AACv2", "LD", "ELD", "Main", "SSR", "LTP"}));
  else
    // Old ffmpeg versions do not parse profiles in the descriptor
    EXPECT_EQ(audioCodecDescriptor->profiles.size(), 0);

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
  auto       demuxer       = openTestFileInDemuxer(openLibraries());
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
      EXPECT_EQ(packet->getDataSize(), expectedDataSizesAudio.at(packetCountAudio));
      ++packetCountAudio;
    }
    else if (packet->getStreamIndex() == 1)
    {
      EXPECT_EQ(packet->getDataSize(), expectedDataSizesVideo.at(packetCountVideo));
      ++packetCountVideo;
    }
  }

  EXPECT_EQ(packetCountAudio, 45);
  EXPECT_EQ(packetCountVideo, 25);
}

TEST(FFmpegTest, DecodingTest)
{
  const auto errorCode = toReturnCode(526789);

  auto ffmpegLibraries = openLibraries();
  auto demuxer         = openTestFileInDemuxer(ffmpegLibraries);
  auto decoder         = Decoder(ffmpegLibraries);

  const auto streamToDecode = 1;
  const auto stream         = demuxer.getFormatContext()->getStream(streamToDecode);

  decoder.openForDecoding(stream);

  auto totalFrameCounter = 0;

  auto pullFramesFromDecoder = [&decoder, &totalFrameCounter]()
  {
    int framesDecodedInLoop = 0;
    while (const auto frame = decoder.decodeNextFrame())
    {
      EXPECT_TRUE(frame);

      EXPECT_EQ(frame->getPixelFormatDescriptor().name, "yuv420p");

      EXPECT_EQ(frame->getSize(), Size({320, 240}));
      EXPECT_EQ(frame->getLineSize(0), 384);
      EXPECT_EQ(frame->getLineSize(1), 192);
      EXPECT_EQ(frame->getLineSize(2), 192);
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
      EXPECT_EQ(frame->getPTS(), expectedPTSValues.at(absoluteFrameIndex));

      constexpr std::array<std::size_t, 25> expectedFrameHashes = {
          10335300354773531646_sz, 9598839882643808065_sz,  6359550546723864943_sz,
          12130793413653774513_sz, 4628923352507498026_sz,  2858635716901433640_sz,
          14970121357180727058_sz, 253752768086499004_sz,   5299985766580468689_sz,
          3099208567159189892_sz,  2133379519116086084_sz,  2180065343928270602_sz,
          2489343340331754166_sz,  5632195775965507548_sz,  3303234774067442630_sz,
          12498746788431539799_sz, 14512385209203951550_sz, 18204733184941058056_sz,
          10739588934149693643_sz, 14470189926003804569_sz, 9627428302243774942_sz,
          11993285150246447462_sz, 15648684513962840504_sz, 15210735267164473200_sz,
          1388011374301074777_sz};
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
