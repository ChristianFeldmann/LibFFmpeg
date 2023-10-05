/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include "AVFormatContextWrapper.h"
#include "AVStreamWrapper.h"

namespace LibFFmpeg
{

namespace
{

// AVFormatContext is part of avformat.
// These functions give us version independent access to the structs.
typedef struct AVFormatContext_56
{
  const AVClass *        av_class;
  struct AVInputFormat * iformat;
  struct AVOutputFormat *oformat;
  void *                 priv_data;
  AVIOContext *          pb;
  int                    ctx_flags;
  unsigned int           nb_streams; //
  AVStream **            streams;    //
  char                   filename[1024];
  int64_t                start_time;
  int64_t                duration; //
  int                    bit_rate;
  unsigned int           packet_size;
  int                    max_delay;
  int                    flags;
  unsigned int           probesize;
  int                    max_analyze_duration;
  const uint8_t *        key;
  int                    keylen;
  unsigned int           nb_programs;
  AVProgram **           programs;
  enum AVCodecID         video_codec_id;
  enum AVCodecID         audio_codec_id;
  enum AVCodecID         subtitle_codec_id;
  unsigned int           max_index_size;
  unsigned int           max_picture_buffer;
  unsigned int           nb_chapters;
  AVChapter **           chapters;
  AVDictionary *         metadata;

  // Actually, there is more here, but the variables above are the only we need.
} AVFormatContext_56;

typedef struct AVFormatContext_57
{
  const AVClass *        av_class;
  struct AVInputFormat * iformat;
  struct AVOutputFormat *oformat;
  void *                 priv_data;
  AVIOContext *          pb;
  int                    ctx_flags;
  unsigned int           nb_streams;
  AVStream **            streams;
  char                   filename[1024];
  int64_t                start_time;
  int64_t                duration;
  int64_t                bit_rate;
  unsigned int           packet_size;
  int                    max_delay;
  int                    flags;
  unsigned int           probesize;
  int                    max_analyze_duration;
  const uint8_t *        key;
  int                    keylen;
  unsigned int           nb_programs;
  AVProgram **           programs;
  enum AVCodecID         video_codec_id;
  enum AVCodecID         audio_codec_id;
  enum AVCodecID         subtitle_codec_id;
  unsigned int           max_index_size;
  unsigned int           max_picture_buffer;
  unsigned int           nb_chapters;
  AVChapter **           chapters;
  AVDictionary *         metadata;

  // Actually, there is more here, but the variables above are the only we need.
} AVFormatContext_57;

typedef struct AVFormatContext_58
{
  const AVClass *        av_class;
  struct AVInputFormat * iformat;
  struct AVOutputFormat *oformat;
  void *                 priv_data;
  AVIOContext *          pb;
  int                    ctx_flags;
  unsigned int           nb_streams;
  AVStream **            streams;
  char                   filename[1024];
  char *                 url;
  int64_t                start_time;
  int64_t                duration;
  int64_t                bit_rate;
  unsigned int           packet_size;
  int                    max_delay;
  int                    flags;
  int64_t                probesize;
  int64_t                max_analyze_duration;
  const uint8_t *        key;
  int                    keylen;
  unsigned int           nb_programs;
  AVProgram **           programs;
  enum AVCodecID         video_codec_id;
  enum AVCodecID         audio_codec_id;
  enum AVCodecID         subtitle_codec_id;
  unsigned int           max_index_size;
  unsigned int           max_picture_buffer;
  unsigned int           nb_chapters;
  AVChapter **           chapters;
  AVDictionary *         metadata;

  // Actually, there is more here, but the variables above are the only we need.
} AVFormatContext_58;

typedef struct AVFormatContext_59_60
{
  const AVClass *        av_class;
  struct AVInputFormat * iformat;
  struct AVOutputFormat *oformat;
  void *                 priv_data;
  AVIOContext *          pb;
  int                    ctx_flags;
  unsigned int           nb_streams;
  AVStream **            streams;
  char *                 url;
  int64_t                start_time;
  int64_t                duration;
  int64_t                bit_rate;
  unsigned int           packet_size;
  int                    max_delay;
  int                    flags;
  int64_t                probesize;
  int64_t                max_analyze_duration;
  const uint8_t *        key;
  int                    keylen;
  unsigned int           nb_programs;
  AVProgram **           programs;
  enum AVCodecID         video_codec_id;
  enum AVCodecID         audio_codec_id;
  enum AVCodecID         subtitle_codec_id;
  unsigned int           max_index_size;
  unsigned int           max_picture_buffer;
  unsigned int           nb_chapters;
  AVChapter **           chapters;
  AVDictionary *         metadata;

  // Actually, there is more here, but the variables above are the only we need.
} AVFormatContext_59_60;

} // namespace

AVFormatContextWrapper::AVFormatContextWrapper(AVFormatContext *      c,
                                               const LibraryVersions &libraryVersions)
{
  this->ctx             = c;
  this->libraryVersions = libraryVersions;
  this->update();
}

void AVFormatContextWrapper::updateFrom(AVFormatContext *c)
{
  assert(this->ctx == nullptr);
  this->ctx = c;
  this->update();
}

AVFormatContextWrapper::operator bool() const
{
  return this->ctx;
};

unsigned AVFormatContextWrapper::getNbStreams()
{
  this->update();
  return this->nb_streams;
}

AVStreamWrapper AVFormatContextWrapper::getStream(int idx)
{
  this->update();
  return this->streams[idx];
}

AVInputFormatWrapper AVFormatContextWrapper::getInputFormat()
{
  this->update();
  return this->iformat;
}

int64_t AVFormatContextWrapper::getStartTime()
{
  this->update();
  return this->start_time;
}

int64_t AVFormatContextWrapper::getDuration()
{
  this->update();
  return this->duration;
}

AVFormatContext *AVFormatContextWrapper::getFormatCtx() const
{
  return this->ctx;
}

AVDictionaryWrapper AVFormatContextWrapper::getMetadata()
{
  this->update();
  return this->metadata;
}

void AVFormatContextWrapper::update()
{
  if (this->ctx == nullptr)
    return;

  this->streams.clear();

  // Copy values from the source pointer
  if (this->libraryVersions.avformat.major == 56)
  {
    auto p           = reinterpret_cast<AVFormatContext_56 *>(this->ctx);
    this->ctx_flags  = p->ctx_flags;
    this->nb_streams = p->nb_streams;
    for (unsigned i = 0; i < this->nb_streams; i++)
      this->streams.push_back(AVStreamWrapper(p->streams[i], this->libraryVersions));
    this->filename             = std::string(p->filename);
    this->start_time           = p->start_time;
    this->duration             = p->duration;
    this->bit_rate             = p->bit_rate;
    this->packet_size          = p->packet_size;
    this->max_delay            = p->max_delay;
    this->flags                = p->flags;
    this->probesize            = p->probesize;
    this->max_analyze_duration = p->max_analyze_duration;
    this->key                  = std::string((const char *)p->key, p->keylen);
    this->nb_programs          = p->nb_programs;
    this->video_codec_id       = p->video_codec_id;
    this->audio_codec_id       = p->audio_codec_id;
    this->subtitle_codec_id    = p->subtitle_codec_id;
    this->max_index_size       = p->max_index_size;
    this->max_picture_buffer   = p->max_picture_buffer;
    this->nb_chapters          = p->nb_chapters;
    this->metadata             = AVDictionaryWrapper(p->metadata);

    this->iformat = AVInputFormatWrapper(p->iformat, this->libraryVersions);
  }
  else if (this->libraryVersions.avformat.major == 57)
  {
    auto p           = reinterpret_cast<AVFormatContext_57 *>(this->ctx);
    this->ctx_flags  = p->ctx_flags;
    this->nb_streams = p->nb_streams;
    for (unsigned i = 0; i < nb_streams; i++)
      this->streams.push_back(AVStreamWrapper(p->streams[i], this->libraryVersions));
    this->filename             = std::string(p->filename);
    this->start_time           = p->start_time;
    this->duration             = p->duration;
    this->bit_rate             = p->bit_rate;
    this->packet_size          = p->packet_size;
    this->max_delay            = p->max_delay;
    this->flags                = p->flags;
    this->probesize            = p->probesize;
    this->max_analyze_duration = p->max_analyze_duration;
    this->key                  = std::string((const char *)p->key, p->keylen);
    this->nb_programs          = p->nb_programs;
    this->video_codec_id       = p->video_codec_id;
    this->audio_codec_id       = p->audio_codec_id;
    this->subtitle_codec_id    = p->subtitle_codec_id;
    this->max_index_size       = p->max_index_size;
    this->max_picture_buffer   = p->max_picture_buffer;
    this->nb_chapters          = p->nb_chapters;
    this->metadata             = AVDictionaryWrapper(p->metadata);

    this->iformat = AVInputFormatWrapper(p->iformat, this->libraryVersions);
  }
  else if (this->libraryVersions.avformat.major == 58)
  {
    auto p           = reinterpret_cast<AVFormatContext_58 *>(this->ctx);
    this->ctx_flags  = p->ctx_flags;
    this->nb_streams = p->nb_streams;
    for (unsigned i = 0; i < nb_streams; i++)
      this->streams.push_back(AVStreamWrapper(p->streams[i], this->libraryVersions));
    this->filename             = std::string(p->filename);
    this->start_time           = p->start_time;
    this->duration             = p->duration;
    this->bit_rate             = p->bit_rate;
    this->packet_size          = p->packet_size;
    this->max_delay            = p->max_delay;
    this->flags                = p->flags;
    this->probesize            = p->probesize;
    this->max_analyze_duration = p->max_analyze_duration;
    this->key                  = std::string((const char *)p->key, p->keylen);
    this->nb_programs          = p->nb_programs;
    this->video_codec_id       = p->video_codec_id;
    this->audio_codec_id       = p->audio_codec_id;
    this->subtitle_codec_id    = p->subtitle_codec_id;
    this->max_index_size       = p->max_index_size;
    this->max_picture_buffer   = p->max_picture_buffer;
    this->nb_chapters          = p->nb_chapters;
    this->metadata             = AVDictionaryWrapper(p->metadata);

    this->iformat = AVInputFormatWrapper(p->iformat, this->libraryVersions);
  }
  else if (this->libraryVersions.avformat.major == 59 || //
           this->libraryVersions.avformat.major == 60)
  {
    auto p           = reinterpret_cast<AVFormatContext_59_60 *>(this->ctx);
    this->ctx_flags  = p->ctx_flags;
    this->nb_streams = p->nb_streams;
    for (unsigned i = 0; i < nb_streams; i++)
      this->streams.push_back(AVStreamWrapper(p->streams[i], this->libraryVersions));
    this->filename             = std::string(p->url);
    this->start_time           = p->start_time;
    this->duration             = p->duration;
    this->bit_rate             = p->bit_rate;
    this->packet_size          = p->packet_size;
    this->max_delay            = p->max_delay;
    this->flags                = p->flags;
    this->probesize            = p->probesize;
    this->max_analyze_duration = p->max_analyze_duration;
    this->key                  = std::string((const char *)p->key, p->keylen);
    this->nb_programs          = p->nb_programs;
    this->video_codec_id       = p->video_codec_id;
    this->audio_codec_id       = p->audio_codec_id;
    this->subtitle_codec_id    = p->subtitle_codec_id;
    this->max_index_size       = p->max_index_size;
    this->max_picture_buffer   = p->max_picture_buffer;
    this->nb_chapters          = p->nb_chapters;
    this->metadata             = AVDictionaryWrapper(p->metadata);

    this->iformat = AVInputFormatWrapper(p->iformat, this->libraryVersions);
  }
  else
    throw std::runtime_error("Invalid library version");
}

} // namespace LibFFmpeg
