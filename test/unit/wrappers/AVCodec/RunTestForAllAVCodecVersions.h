/* Copyright (c) 2023 Christian Feldmann [christian.feldmann@gmx.de].
 * All rights reserved.
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#pragma once

#define RUN_TEST_FOR_ALL_AVCODEC_VERSIONS(testFunctionName, classPrefix)                           \
  {                                                                                                \
    const auto version = GetParam();                                                               \
    if (version.avcodec.major == 56)                                                               \
      testFunctionName<classPrefix##_56>(version);                                                 \
    else if (version.avcodec.major == 57)                                                          \
      testFunctionName<classPrefix##_57>(version);                                                 \
    else if (version.avcodec.major == 58)                                                          \
      testFunctionName<classPrefix##_58>(version);                                                 \
    else if (version.avcodec.major == 59)                                                          \
      testFunctionName<classPrefix##_59>(version);                                                 \
    else if (version.avcodec.major == 60)                                                          \
      testFunctionName<classPrefix##_60>(version);                                                 \
  }
