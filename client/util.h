// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#ifndef HEX_CLIENT_UTIL_H_
#define HEX_CLIENT_UTIL_H_
#pragma once

#include "include/cef_task.h"

#if defined(OS_WIN)

#include <windows.h>  // NOLINT(build/include_order)

#ifndef NDEBUG
#define ASSERT(condition) if (!(condition)) { DebugBreak(); }
#else
#define ASSERT(condition) ((void)0)
#endif

#else  // !OS_WIN

#include <assert.h>  // NOLINT(build/include_order)

#ifndef NDEBUG
#define ASSERT(condition) if (!(condition)) { assert(false); }
#else
#define ASSERT(condition) ((void)0)
#endif

#endif  // !OS_WIN

#define REQUIRE_UI_THREAD()   ASSERT(CefCurrentlyOn(TID_UI));
#define REQUIRE_IO_THREAD()   ASSERT(CefCurrentlyOn(TID_IO));
#define REQUIRE_FILE_THREAD() ASSERT(CefCurrentlyOn(TID_FILE));

namespace hexclient {

void InitUtil();

// Returns the full path of the filename in the execution directory.
CefString GetFullPathInExecutionDirectory(CefString fileName,
                                          bool includingSlash);

// Returns the application working directory.
CefString GetWorkingDirectory();

// Returns the application execution directory.
CefString GetExecutionDirectory();

// Translate the "$(AppDir)" into the full path in the execution directory.
CefString ProcessAppDir(std::string filePath);

// Determine if the specified port is used.
bool IsPortUsed(int port);

}  // namespace hexclient

#endif  // HEX_CLIENT_UTIL_H_
