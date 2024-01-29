/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/Version.h>
#include <libHandling/IFFmpegLibraries.h>

#include <gmock/gmock.h>

namespace ffmpeg
{

struct AVDummy
{
  int dummyInt{};
};

class FFmpegLibrariesMock : public IFFmpegLibraries
{
public:
  FFmpegLibrariesMock();
  ~FFmpegLibrariesMock();

  MOCK_METHOD(std::vector<LibraryInfo>, getLibrariesInfo, (), (const, override));
  MOCK_METHOD(LibraryVersions, getLibrariesVersion, (), (const, override));

  MOCK_METHOD(std::string_view, getLogList, (), (const, override));

  struct FunctionCounters
  {
    int avFrameAlloc{};
    int avFrameFree{};
    int avPixFmtDescGet{};
    int avDictGet{};
    int avPacketAlloc{};
    int avPacketFree{};
    int avFreePacket{};
    int avInitPacket{};
    int avcodecReceiveFrame{};
    int avcodecSendPacketNonNull{};
    int avcodecSendPacketNull{};
    int avcodecDescriptorGet{};
    int avcodecFindDecoder{};
    int avcodecAllocContext3{};
    int avcodecFreeContext{};
    int avcodecParametersToContext{};
    int avcodecOpen2{};
  };
  FunctionCounters functionCounters{};

  struct FunctionCallValues
  {
    std::vector<internal::AVPixelFormat> avPixFmtDescGet;
  };
  FunctionCallValues functionCallValues;

  struct FunctionChecks
  {
    std::optional<internal::AVCodecID>     avcodecFindDecoderExpectedCodecID{};
    std::optional<internal::AVCodecID>     avcodecDescriptorGetExpectedID{};
    std::optional<internal::AVPixelFormat> avutilPixFmtDescGetExpectedFormat{};
  };
  FunctionChecks functionChecks;

private:
  AVDummy avDummy;

  // AVUtil
  internal::AVFrame                  *av_frame_alloc_mock();
  void                                av_frame_free_mock(internal::AVFrame **frame);
  const internal::AVPixFmtDescriptor *av_pix_fmt_desc_get_mock(internal::AVPixelFormat pix_fmt);
  internal::AVDictionaryEntry        *av_dict_get_moc(internal::AVDictionary            *dictionary,
                                                      const char                        *key,
                                                      const internal::AVDictionaryEntry *prev,
                                                      int                                flags);

  // AVCodec
  internal::AVPacket *av_packet_alloc_mock();
  void                av_packet_free_mock(internal::AVPacket **packet);
  void                av_free_packet_mock(internal::AVPacket *packet);
  void                av_init_packet_mock(internal::AVPacket *packet);
  int avcodec_receive_frame_mock(internal::AVCodecContext *context, internal::AVFrame *frame);
  int avcodec_send_packet_mock(internal::AVCodecContext *context, const internal::AVPacket *packet);
  const internal::AVCodecDescriptor *avcodec_descriptor_get_mock(internal::AVCodecID codecID);
  internal::AVCodec                 *avcodec_find_decoder_mock(internal::AVCodecID codecID);
  internal::AVCodecContext          *avcodec_alloc_context3_mock(const internal::AVCodec *codec);
  void avcodec_free_context_mock(internal::AVCodecContext **codecContext);
  int  avcodec_parameters_to_context_mock(internal::AVCodecContext *,
                                          const internal::AVCodecParameters *);
  int  avcodec_open2_mock(internal::AVCodecContext *,
                          const internal::AVCodec *,
                          internal::AVDictionary **);
};

} // namespace ffmpeg
