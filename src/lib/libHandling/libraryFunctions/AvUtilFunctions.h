/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#include <common/FFMpegLibrariesTypes.h>
#include <libHandling/SharedLibraryLoader.h>

namespace ffmpeg::functions
{

struct AvUtilFunctions
{
  std::function<unsigned()>            avutil_version;
  std::function<AVFrame *()>           av_frame_alloc;
  std::function<void(AVFrame **frame)> av_frame_free;
  std::function<void *(size_t size)>   av_mallocz;
  std::function<void(void *ptr)>       av_freep;
  std::function<int(AVDictionary **pm, const char *key, const char *value, int flags)> av_dict_set;
  std::function<AVDictionaryEntry *(
      AVDictionary *m, const char *key, const AVDictionaryEntry *prev, int flags)>
      av_dict_get;
  std::function<AVFrameSideData *(const AVFrame *frame, AVFrameSideDataType type)>
                                                                            av_frame_get_side_data;
  std::function<AVDictionary *(const AVFrame *frame)>                       av_frame_get_metadata;
  std::function<void(void (*callback)(void *, int, const char *, va_list))> av_log_set_callback;
  std::function<void(int level)>                                            av_log_set_level;
  std::function<AVPixFmtDescriptor *(AVPixelFormat pix_fmt)>                av_pix_fmt_desc_get;
  std::function<AVPixFmtDescriptor *(const AVPixFmtDescriptor *prev)>       av_pix_fmt_desc_next;
  std::function<AVPixelFormat(const AVPixFmtDescriptor *desc)>              av_pix_fmt_desc_get_id;
};

std::optional<AvUtilFunctions> tryBindAVUtilFunctionsFromLibrary(SharedLibraryLoader &lib,
                                                                 Log                 &log);

} // namespace ffmpeg::functions
