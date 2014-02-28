// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#ifndef HEX_SRC_HEX_EXPORT_H_
#define HEX_SRC_HEX_EXPORT_H_

#if defined(HEXCLIENT)
#include "cef/include/internal/cef_build.h"
#else
#include "build/build_config.h"
#endif  // HEXCLIENT

#if defined(BUILDING_HEX_SHARED)
#if defined(OS_WIN)

#if defined(HEX_IMPLEMENTATION)
#define HEX_EXPORT __declspec(dllexport)
#else
#define HEX_EXPORT __declspec(dllimport)
#endif  // defined(HEX_IMPLEMENTATION)

#else  // defined(OS_WIN)
#if defined(HEX_IMPLEMENTATION)
#define HEX_EXPORT __attribute__((visibility("default")))
#else
#define HEX_EXPORT
#endif  // defined(HEX_IMPLEMENTATION)
#endif

#else  // defined(BUILDING_HEX_SHARED)
#define HEX_EXPORT
#endif

#endif  // HEX_SRC_HEX_EXPORT_H_
