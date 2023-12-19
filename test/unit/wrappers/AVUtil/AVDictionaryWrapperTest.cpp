/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVUtil/wrappers/AVDictionaryWrapper.h>
#include <common/InternalTypes.h>
#include <libHandling/FFmpegLibrariesMoc.h>

#include <gtest/gtest.h>

namespace ffmpeg::avutil
{

using ffmpeg::internal::AVDictionary;
using ffmpeg::internal::AVDictionaryEntry;
using StringPair = std::pair<std::string, std::string>;

namespace
{

constexpr auto AV_DICT_IGNORE_SUFFIX = 2;

// We emulate what this function does in ffmpeg (roughly)
AVDictionaryEntry *
avDictGet(AVDictionary *dictionray, const char *key, const AVDictionaryEntry *prev, int flags)
{
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

} // namespace

TEST(AVFrameSideDataWrapperTest, NullptrShouldReturnEmptyMap)
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();

  AVDictionaryWrapper wrapper(nullptr, ffmpegLibraries);
  EXPECT_TRUE(wrapper.toMap().empty());
}

TEST(AVFrameSideDataWrapperTest, GetDictEntriesTest)
{
  auto ffmpegLibraries                = std::make_shared<FFmpegLibrariesMock>();
  ffmpegLibraries->avutil.av_dict_get = avDictGet;

  std::array<AVDictionaryEntry, 5> dummyEntries = {
      AVDictionaryEntry({(char *)"key1", (char *)"value1"}),
      AVDictionaryEntry({(char *)"key2", (char *)"value2"}),
      AVDictionaryEntry({(char *)"key3", (char *)"value3"}),
      AVDictionaryEntry({(char *)"key4", (char *)"value4"}),
      AVDictionaryEntry({nullptr, nullptr})};

  // The actual pointer is never used in this test. It just must not be nullptr.
  const auto dictPtr = reinterpret_cast<AVDictionary *>(dummyEntries.data());

  AVDictionaryWrapper wrapper(dictPtr, ffmpegLibraries);
  const auto          map = wrapper.toMap();
  EXPECT_EQ(map.size(), 4);

  EXPECT_EQ(map.at("key1"), "value1");
  EXPECT_EQ(map.at("key2"), "value2");
  EXPECT_EQ(map.at("key3"), "value3");
  EXPECT_EQ(map.at("key4"), "value4");
}

} // namespace ffmpeg::avutil