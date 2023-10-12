/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/FFMpegLibrariesTypes.h>
#include <libHandling/FFmpegLibrariesInterface.h>

#include <memory>

namespace ffmpeg::avcodec
{

// AVPacket is part of avcodec. The definition is different for different major versions of avcodec.
// These are the version independent functions to retrive data from AVPacket.
// The size of this struct is part of the public API and must be correct
// since its size is used in other structures (e.g. AVStream).
struct AVPacket_56
{
  AVBufferRef      *buf;
  int64_t           pts;
  int64_t           dts;
  uint8_t          *data;
  int               size;
  int               stream_index;
  int               flags;
  AVPacketSideData *side_data;
  int               side_data_elems;
  int               duration;
  void (*destruct)(struct AVPacket *);
  void   *priv;
  int64_t pos;
};

struct AVPacket_57
{
  AVBufferRef      *buf;
  int64_t           pts;
  int64_t           dts;
  uint8_t          *data;
  int               size;
  int               stream_index;
  int               flags;
  AVPacketSideData *side_data;
  int               side_data_elems;
  int64_t           duration;
  int64_t           pos;
  int64_t           convergence_duration;
};

typedef AVPacket_57 AVPacket_58;

struct AVPacket_59
{
  AVBufferRef      *buf;
  int64_t           pts;
  int64_t           dts;
  uint8_t          *data;
  int               size;
  int               stream_index;
  int               flags;
  AVPacketSideData *side_data;
  int               side_data_elems;
  int64_t           duration;
  int64_t           pos;
  void             *opaque;
  AVBufferRef      *opaque_ref;
  AVRational        time_base;
};

typedef AVPacket_59 AVPacket_60;

// A wrapper around the different versions of the AVPacket versions.
// It also adds some functions like automatic deletion when it goes out of scope.
class AVPacketWrapper
{
public:
  AVPacketWrapper() = default;
  AVPacketWrapper(AVPacket *packet, std::shared_ptr<FFmpegLibrariesInterface> librariesInterface);

  void setData(const ByteVector &data);
  void setTimestamps(const int64_t dts, const int64_t pts);

  AVPacket *getPacket() const { return this->packet; }

  struct Flags
  {
    bool keyframe{};
    bool corrupt{};
    bool discard{};
  };

  int        getStreamIndex() const;
  int64_t    getPTS() const;
  int64_t    getDTS() const;
  int64_t    getDuration() const;
  Flags      getFlags() const;
  ByteVector getData() const;

  explicit operator bool() const { return this->packet != nullptr; };

private:
  AVPacket                                 *packet{};
  std::shared_ptr<FFmpegLibrariesInterface> librariesInterface{};
};

} // namespace ffmpeg::avcodec
