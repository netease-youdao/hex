// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#ifndef HEX_SRC_CUSTOMIZED_H_
#define HEX_SRC_CUSTOMIZED_H_
#pragma once

#include "include/cef_base.h"
#include "include/cef_browser.h"

#define EmitNodeEvent(browser, name, format, ...) WCHAR script[MAX_PATH];\
wsprintf(script, L"(function() { var event = document.createEvent('CustomEvent'); event.initCustomEvent('"##name##L"', false, true, "##format##L"); window.dispatchEvent(event); })();", ##__VA_ARGS__);\
hex::RunJavaScriptCode(browser, script);

#define EmitMouseEvent(browser, name) WCHAR script[MAX_PATH];\
wsprintf(script, L"(function() { var event = document.createEvent('MouseEvent'); event.initMouseEvent('"##name##L"', true, true, window, 0, 0, 0, 0, 0, false, false, false, false, 0, null); document.dispatchEvent(event); })();");\
hex::RunJavaScriptCode(browser, script);

namespace hex {

#if defined(OS_WIN)

// Returns the monitor info.
//MONITORINFO GetMonitorInformation(HWND handle);

/*// Determine if the current OS is Windows XP.
bool IsWindowsXP();

// Determine if the aero glass can be used in the current OS.
bool IsAeroGlassEnabled();

// Apply the aero glass to extend the whole window.
void SetAeroGlass(HWND handle, bool enable_on_maximized);*/

// Runs the javascript code in the main browser window.
void RunJavaScriptCode(CefRefPtr<CefBrowser> browser, CefString code);

#endif  // OS_WIN

}  // namespace hex

#endif  // HEX_SRC_CUSTOMIZED_H_