/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVFrameWrapper.h"

#include "CastUtilClasses.h"

#include <common/InternalTypes.h>

#include <stdexcept>

namespace ffmpeg::avutil
{

namespace
{

struct AVFrame_54
{
  uint8_t *     data[AV_NUM_DATA_POINTERS];
  int           linesize[AV_NUM_DATA_POINTERS];
  uint8_t **    extended_data;
  int           width, height;
  int           nb_samples;
  int           format;
  int           key_frame;
  AVPictureType pict_type;
  uint8_t *     base[AV_NUM_DATA_POINTERS];
  AVRational    sample_aspect_ratio;
  int64_t       pts;
  int64_t       pkt_pts;
  int64_t       pkt_dts;
  int           coded_picture_number;
  int           display_picture_number;
  int           quality;
  // Actually, there is more here, but the variables above are the only we need.
};

struct AVFrame_55
{
  uint8_t *     data[AV_NUM_DATA_POINTERS];
  int           linesize[AV_NUM_DATA_POINTERS];
  uint8_t **    extended_data;
  int           width, height;
  int           nb_samples;
  int           format;
  int           key_frame;
  AVPictureType pict_type;
  AVRational    sample_aspect_ratio;
  int64_t       pts;
  int64_t       pkt_pts;
  int64_t       pkt_dts;
  int           coded_picture_number;
  int           display_picture_number;
  int           quality;
  // Actually, there is more here, but the variables above are the only we need.
};

typedef AVFrame_55 AVFrame_56;

struct AVFrame_57
{
  uint8_t *                          data[AV_NUM_DATA_POINTERS];
  int                                linesize[AV_NUM_DATA_POINTERS];
  uint8_t **                         extended_data;
  int                                width, height;
  int                                nb_samples;
  int                                format;
  int                                key_frame;
  AVPictureType                      pict_type;
  AVRational                         sample_aspect_ratio;
  int64_t                            pts;
  int64_t                            pkt_dts;
  AVRational                         time_base;
  int                                coded_picture_number;
  int                                display_picture_number;
  int                                quality;
  void *                             opaque;
  int                                repeat_pict;
  int                                interlaced_frame;
  int                                top_field_first;
  int                                palette_has_changed;
  int64_t                            reordered_opaque;
  int                                sample_rate;
  uint64_t                           channel_layout;
  AVBufferRef *                      buf[AV_NUM_DATA_POINTERS];
  AVBufferRef **                     extended_buf;
  int                                nb_extended_buf;
  AVFrameSideData **                 side_data;
  int                                nb_side_data;
  int                                flags;
  enum AVColorRange                  color_range;
  enum AVColorPrimaries              color_primaries;
  enum AVColorTransferCharacteristic color_trc;
  internal::AVColorSpace             colorspace;
  enum AVChromaLocation              chroma_location;
  int64_t                            best_effort_timestamp;
  int64_t                            pkt_pos;
  int64_t                            pkt_duration;
  AVDictionary *                     metadata;

  // Actually, there is more here, but the variables above are the only we need.
};

typedef AVFrame_57 AVFrame_58;

} // namespace

AVFrameWrapper::AVFrameWrapper(AVFrame *                                 frame,
                               std::shared_ptr<FFmpegLibrariesInterface> librariesInterface)
    : frame(frame), librariesInterface(librariesInterface)
{
}

ByteVector AVFrameWrapper::getData(int component) const
{
  uint8_t *dataPointer;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame, dataPointer, data[component]);

  ByteVector data;
  // Data copying function

  return data;
}

int AVFrameWrapper::getLineSize(int component) const
{
  if (component < 0 || component > AV_NUM_DATA_POINTERS)
    return {};

  int linesize;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame, linesize, linesize[component]);
  return linesize;
}

Size AVFrameWrapper::getSize() const
{
  int width;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame, width, width);

  int height;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame, height, height);

  return {width, height};
}

int64_t AVFrameWrapper::getPTS() const
{
  int64_t pts;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame, pts, pts);
  return pts;
}

AVPictureType AVFrameWrapper::getPictType() const
{
  AVPictureType pictureType;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame, pictureType, pict_type);
  return pictureType;
}

bool AVFrameWrapper::isKeyFrame() const
{
  int keyframe;
  CAST_AVUTIL_GET_MEMBER(AVFrame, this->frame, keyframe, key_frame);
  return keyframe == 1;
}

AVDictionaryWrapper AVFrameWrapper::getMetadata() const
{
  const auto version = this->librariesInterface->getLibrariesVersion().avutil.major;

  if (version == 57 || version == 58)
  {
    const auto p = reinterpret_cast<AVFrame_57 *>(this->frame);
    return AVDictionaryWrapper(p->metadata, this->librariesInterface);
  }

  return {};
}

} // namespace ffmpeg::avutil
