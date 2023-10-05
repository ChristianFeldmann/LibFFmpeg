/* LibFFmpeg++
 * Copyright (C) 2023 Christian Feldmann [christian.feldmann@gmx.de]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <common/Expected.h>
#include <common/FFMpegLibrariesTypes.h>
#include <libHandling/SharedLibraryLoader.h>

namespace LibFFmpeg::functions
{

struct AvUtilFunctions
{
  std::function<unsigned()>            avutil_version;
  std::function<AVFrame *()>           av_frame_alloc;
  std::function<void(AVFrame **frame)> av_frame_free;
  std::function<void(size_t size)>     av_mallocz;
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
                                                                 Log &                log);

} // namespace LibFFmpeg::functions
