/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "FFmpegLibrariesMoc.h"

namespace ffmpeg
{

using internal::AVCodec;
using internal::AVCodecContext;
using internal::AVCodecDescriptor;
using internal::AVCodecID;
using internal::AVCodecParameters;
using internal::AVDictionary;
using internal::AVDictionaryEntry;
using internal::AVFrame;
using internal::AVPacket;
using internal::AVPixelFormat;
using internal::AVPixFmtDescriptor;

FFmpegLibrariesMock::FFmpegLibrariesMock()
{
  this->avutil.av_frame_alloc      = [this]() { return this->av_frame_alloc_mock(); };
  this->avutil.av_frame_free       = [this](AVFrame **frame) { this->av_frame_free_mock(frame); };
  this->avutil.av_pix_fmt_desc_get = [this](AVPixelFormat pix_fmt)
  { return this->av_pix_fmt_desc_get_mock(pix_fmt); };
  this->avutil.av_dict_get =
      [this](AVDictionary *dictionray, const char *key, const AVDictionaryEntry *prev, int flags)
  { return this->av_dict_get_moc(dictionray, key, prev, flags); };

  this->avcodec.av_packet_alloc = [this]() { return this->av_packet_alloc_mock(); };
  this->avcodec.av_packet_free  = [this](AVPacket **packet) { this->av_packet_free_mock(packet); };
  this->avcodec.avcodec_receive_frame = [this](AVCodecContext *context, AVFrame *frame)
  { return this->avcodec_receive_frame_mock(context, frame); };
  this->avcodec.avcodec_send_packet = [this](AVCodecContext *context, const AVPacket *packet)
  { return this->avcodec_send_packet_mock(context, packet); };
  this->avcodec.avcodec_descriptor_get = [this](AVCodecID codecID)
  { return this->avcodec_descriptor_get_mock(codecID); };
  this->avcodec.avcodec_find_decoder = [this](AVCodecID codecID)
  { return this->avcodec_find_decoder_mock(codecID); };
  this->avcodec.avcodec_alloc_context3 = [this](const AVCodec *codec)
  { return this->avcodec_alloc_context3_mock(codec); };
  this->avcodec.avcodec_free_context = [this](AVCodecContext **codecContext)
  { this->avcodec_free_context_mock(codecContext); };
  this->avcodec.avcodec_parameters_to_context =
      [this](AVCodecContext *codecContext, const AVCodecParameters *codecParameters)
  { return this->avcodec_parameters_to_context_mock(codecContext, codecParameters); };
  this->avcodec.avcodec_open2 = [this](internal::AVCodecContext *codecContext,
                                       const internal::AVCodec  *codec,
                                       internal::AVDictionary  **dict)
  { return this->avcodec_open2_mock(codecContext, codec, dict); };
}

FFmpegLibrariesMock::~FFmpegLibrariesMock()
{
  EXPECT_EQ(this->functionCounters.avFrameAlloc, this->functionCounters.avFrameFree)
      << "Not all allocated frames were freed again";
  EXPECT_EQ(this->functionCounters.avPacketAlloc, this->functionCounters.avPacketFree)
      << "Not all allocated packets were freed again";
  EXPECT_EQ(this->functionCounters.avcodecAllocContext3, this->functionCounters.avcodecFreeContext)
      << "Not all allocated codec contexts were freed again";
}

AVFrame *FFmpegLibrariesMock::av_frame_alloc_mock()
{
  auto frame = new AVDummy;
  ++this->functionCounters.avFrameAlloc;
  return reinterpret_cast<AVFrame *>(frame);
}

void FFmpegLibrariesMock::av_frame_free_mock(AVFrame **frame)
{
  if (frame != nullptr && *frame != nullptr)
  {
    auto castFrame = reinterpret_cast<AVDummy *>(*frame);
    delete (castFrame);
    *frame = nullptr;
    ++this->functionCounters.avFrameFree;
  }
}

const AVPixFmtDescriptor *FFmpegLibrariesMock::av_pix_fmt_desc_get_mock(AVPixelFormat pix_fmt)
{
  if (this->functionChecks.avutilPixFmtDescGetExpectedFormat)
    EXPECT_EQ(pix_fmt, this->functionChecks.avutilPixFmtDescGetExpectedFormat.value());
  ++this->functionCounters.avPixFmtDescGet;
  this->functionCallValues.avPixFmtDescGet.push_back(pix_fmt);
  return nullptr;
}

AVDictionaryEntry *FFmpegLibrariesMock::av_dict_get_moc(AVDictionary            *dictionray,
                                                        const char              *key,
                                                        const AVDictionaryEntry *prev,
                                                        int                      flags)
{
  constexpr auto AV_DICT_IGNORE_SUFFIX = 2;

  ++this->functionCounters.avDictGet;

  if (dictionray == nullptr || key == nullptr || flags != AV_DICT_IGNORE_SUFFIX)
    return {};

  auto castBack = reinterpret_cast<AVDictionaryEntry *>(dictionray);
  if (prev == nullptr)
    return castBack;

  while (castBack->key != nullptr && castBack->value != nullptr)
  {
    const auto returnNext = (castBack == prev);
    ++castBack;
    if (returnNext)
      return (castBack->key == nullptr) ? nullptr : castBack;
  }

  return nullptr;
}

AVPacket *FFmpegLibrariesMock::av_packet_alloc_mock()
{
  auto packet = new AVDummy;
  ++this->functionCounters.avPacketAlloc;
  return reinterpret_cast<AVPacket *>(packet);
}

void FFmpegLibrariesMock::av_packet_free_mock(AVPacket **packet)
{
  if (packet != nullptr && *packet != nullptr)
  {
    auto castPacket = reinterpret_cast<AVDummy *>(*packet);
    delete (castPacket);
    *packet = nullptr;
    ++this->functionCounters.avPacketFree;
  }
}

int FFmpegLibrariesMock::avcodec_receive_frame_mock(AVCodecContext *context, AVFrame *frame)
{
  if (context == nullptr)
    throw std::runtime_error("Context must not be null");
  if (frame == nullptr)
    throw std::runtime_error("Frame must not be null");
  ++this->functionCounters.avcodecReceiveFrame;

  return 0;
}

int FFmpegLibrariesMock::avcodec_send_packet_mock(AVCodecContext *context, const AVPacket *packet)
{
  if (context == nullptr)
    throw std::runtime_error("Context must not be null");

  if (packet)
    ++this->functionCounters.avcodecSendPacketNonNull;
  else
    ++this->functionCounters.avcodecSendPacketNull;

  return 0;
}

const AVCodecDescriptor *FFmpegLibrariesMock::avcodec_descriptor_get_mock(AVCodecID codecID)
{
  if (this->functionChecks.avcodecDescriptorGetExpectedID)
    EXPECT_EQ(codecID, this->functionChecks.avcodecDescriptorGetExpectedID.value());
  ++this->functionCounters.avcodecDescriptorGet;
  return nullptr;
}

AVCodec *FFmpegLibrariesMock::avcodec_find_decoder_mock(AVCodecID codecID)
{
  if (this->functionChecks.avcodecFindDecoderExpectedCodecID)
    EXPECT_EQ(codecID, this->functionChecks.avcodecFindDecoderExpectedCodecID.value());
  ++this->functionCounters.avcodecFindDecoder;
  return reinterpret_cast<AVCodec *>(&this->avDummy);
}

internal::AVCodecContext *
FFmpegLibrariesMock::avcodec_alloc_context3_mock(const internal::AVCodec *codec)
{
  EXPECT_NE(codec, nullptr);
  ++this->functionCounters.avcodecAllocContext3;
  auto dummyCodec = new AVDummy;
  return reinterpret_cast<AVCodecContext *>(dummyCodec);
}

void FFmpegLibrariesMock::avcodec_free_context_mock(internal::AVCodecContext **codecContext)
{
  if (codecContext != nullptr && *codecContext != nullptr)
  {
    auto castCodecContext = reinterpret_cast<AVDummy *>(*codecContext);
    delete (castCodecContext);
    *codecContext = nullptr;
    ++this->functionCounters.avPacketFree;
  }
}

int FFmpegLibrariesMock::avcodec_parameters_to_context_mock(
    internal::AVCodecContext *codecContext, const internal::AVCodecParameters *codecParameters)
{
  EXPECT_NE(codecContext, nullptr);
  EXPECT_NE(codecParameters, nullptr);
  ++this->functionCounters.avcodecParametersToContext;
  return 0;
}

int FFmpegLibrariesMock::avcodec_open2_mock(internal::AVCodecContext *codecContext,
                                            const internal::AVCodec  *codec,
                                            internal::AVDictionary  **dictionary)
{
  EXPECT_NE(codecContext, nullptr);
  EXPECT_NE(codec, nullptr);
  ++this->functionCounters.avcodecOpen2;
  return 0;
}

} // namespace ffmpeg
