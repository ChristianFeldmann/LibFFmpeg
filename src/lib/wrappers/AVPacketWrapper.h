/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/FFMpegLibrariesTypes.h>

namespace LibFFmpeg
{

// AVPacket is part of avcodec. The definition is different for different major versions of avcodec.
// These are the version independent functions to retrive data from AVPacket.
// The size of this struct is part of the public API and must be correct
// since its size is used in other structures (e.g. AVStream).
typedef struct AVPacket_56
{
  AVBufferRef *     buf;
  int64_t           pts;
  int64_t           dts;
  uint8_t *         data;
  int               size;
  int               stream_index;
  int               flags;
  AVPacketSideData *side_data;
  int               side_data_elems;
  int               duration;
  void (*destruct)(struct AVPacket *);
  void *  priv;
  int64_t pos;
} AVPacket_56;

typedef struct AVPacket_57_58
{
  AVBufferRef *     buf;
  int64_t           pts;
  int64_t           dts;
  uint8_t *         data;
  int               size;
  int               stream_index;
  int               flags;
  AVPacketSideData *side_data;
  int               side_data_elems;
  int64_t           duration;
  int64_t           pos;
  int64_t           convergence_duration;
} AVPacket_57_58;

typedef struct AVPacket_59_60
{
  AVBufferRef *     buf;
  int64_t           pts;
  int64_t           dts;
  uint8_t *         data;
  int               size;
  int               stream_index;
  int               flags;
  AVPacketSideData *side_data;
  int               side_data_elems;
  int64_t           duration;
  int64_t           pos;
  void *            opaque;
  AVBufferRef *     opaque_ref;
  AVRational        time_base;
} AVPacket_59_60;

// A wrapper around the different versions of the AVPacket versions.
// It also adds some functions like automatic deletion when it goes out of scope.
class AVPacketWrapper
{
public:
  AVPacketWrapper() = default;
  AVPacketWrapper(AVPacket *packet, const LibraryVersions &libraryVersions);

  void clear();

  void setData(const ByteVector &data);
  void setPTS(int64_t pts);
  void setDTS(int64_t dts);

  AVPacket *getPacket();
  int       getStreamIndex();
  int64_t   getPTS();
  int64_t   getDTS();
  int64_t   getDuration();
  int       getFlags();
  bool      getFlagKeyframe();
  bool      getFlagCorrupt();
  bool      getFlagDiscard();
  uint8_t * getData();
  int       getDataSize();

  explicit operator bool() const { return this->pkt != nullptr; };

private:
  void update();

  // These are private. Use "update" to update them from the AVFormatContext
  AVBufferRef *     buf{};
  int64_t           pts{};
  int64_t           dts{};
  uint8_t *         data{};
  int               size{};
  int               stream_index{};
  int               flags{};
  AVPacketSideData *side_data{};
  int               side_data_elems{};
  int64_t           duration{};
  int64_t           pos{};

  ByteVector packetData{};

  AVPacket *      pkt{};
  LibraryVersions libraryVersions{};
};

} // namespace LibFFmpeg
