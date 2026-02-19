/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVUtil/MediaType.h>
#include <common/ComparisonFunctions.h>

#include "LibrariesWithLogging.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <array>

namespace libffmpeg::test::integration
{

using libffmpeg::avutil::ColorSpace;
using libffmpeg::avutil::MediaType;
using ::testing::AnyOf;
using ::testing::Contains;
using ::testing::ElementsAre;

TEST(Demuxing, OpenTestFileAndCheckFormat_ShouldHaveCorrectFormat)
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
  if (majorVersion >= 58 && majorVersion < 62)
    // Older (< ffmpeg 4) versions and newer versions (>= 8) will report this flag as false
    expectedAudioProperties.intraOnly = true;
  expectedAudioProperties.lossy = true;
  EXPECT_EQ(audioCodecDescriptor->properties, expectedAudioProperties);
  EXPECT_EQ(audioCodecDescriptor->mimeTypes.size(), 0);

  const auto audioCodecParameters = audioStream.getCodecParameters();
  if (majorVersion <= 56)
    EXPECT_FALSE(audioCodecParameters);
  else
  {
    EXPECT_EQ(audioCodecParameters->getCodecType(), MediaType::Audio);
    EXPECT_EQ(audioCodecParameters->getExtradata().size(), 5);
    EXPECT_EQ(audioCodecParameters->getSize(), Size({0, 0}));
    EXPECT_EQ(audioCodecParameters->getColorspace(), ColorSpace::UNSPECIFIED);
    EXPECT_THAT(audioCodecParameters->getPixelFormat().name, testing::AnyOf("None", "gray"));
    EXPECT_EQ(audioCodecParameters->getSampleAspectRatio(), Rational({0, 1}));
    EXPECT_THAT(audioCodecParameters->getChannelLayout(),
                ElementsAre(avcodec::ChannelInfo({avcodec::Channel::FrontLeft, {}, "FL"}),
                            avcodec::ChannelInfo({avcodec::Channel::FrontRight, {}, "FR"})));
  }

  if (majorVersion == 56)
    // FFmpeg versions 2 does not parse profiles in the descriptor
    EXPECT_EQ(audioCodecDescriptor->profiles.size(), 0);
  else if (majorVersion == 57)
    // For some reason, the "LC" profile is reported twice for FFmpeg 3
    EXPECT_TRUE(areEqual(audioCodecDescriptor->profiles,
                         {"LC", "HE-AAC", "HE-AACv2", "LD", "ELD", "Main", "LC", "SSR", "LTP"}));
  else if (majorVersion < 62)
    EXPECT_TRUE(areEqual(audioCodecDescriptor->profiles,
                         {"LC", "HE-AAC", "HE-AACv2", "LD", "ELD", "Main", "SSR", "LTP"}));
  else
    EXPECT_TRUE(
        areEqual(audioCodecDescriptor->profiles,
                 {"LC", "HE-AAC", "HE-AACv2", "LD", "ELD", "Main", "SSR", "LTP", "xHE-AAC"}));

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

TEST(Demuxing, OpenTestFileAndDemuxPackets_ShouldReturnCorrectPackets)
{
  auto libsAndLogs = LibrariesWithLogging();

  auto       demuxer       = libsAndLogs.openTestFileInDemuxer();

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

TEST(Demuxing, OpenTestFileAndDemuxPackets_ShouldLogDemuxingEventsCorrectly)
{
  auto libsAndLogs = LibrariesWithLogging();

  auto       demuxer       = libsAndLogs.openTestFileInDemuxer();

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

} // namespace libffmpeg::test::integration
