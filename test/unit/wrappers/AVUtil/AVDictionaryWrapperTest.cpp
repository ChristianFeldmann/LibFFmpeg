/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <AVUtil/wrappers/AVDictionaryWrapper.h>
#include <common/InternalTypes.h>
#include <libHandling/FFmpegLibrariesMoc.h>

#include <gtest/gtest.h>

#include <array>

namespace libffmpeg::avutil
{

using libffmpeg::internal::AVDictionary;
using libffmpeg::internal::AVDictionaryEntry;
using StringPair = std::pair<std::string, std::string>;

TEST(AVDictionaryWrapperTest, NullptrShouldReturnEmptyMap)
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();

  AVDictionaryWrapper wrapper(nullptr, ffmpegLibraries);
  EXPECT_TRUE(wrapper.toMap().empty());
}

TEST(AVDictionaryWrapperTest, shouldThrowIfLibraryNotSet)
{
  AVDummy                           dummyDictionary;
  std::shared_ptr<IFFmpegLibraries> ffmpegLibraries;

  EXPECT_THROW(
      AVDictionaryWrapper(reinterpret_cast<AVDictionary *>(&dummyDictionary), ffmpegLibraries),
      std::runtime_error);
}

TEST(AVDictionaryWrapperTest, GetDictEntriesTest)
{
  auto ffmpegLibraries = std::make_shared<FFmpegLibrariesMock>();

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

  EXPECT_EQ(ffmpegLibraries->functionCounters.avDictGet, 5);
}

} // namespace libffmpeg::avutil