// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "customized.h"
#include <dwmapi.h>

namespace hex {

MONITORINFO GetMonitorInformation(HWND handle) {
  HMONITOR monitor;
  MONITORINFO mi;
  RECT rect;
  GetWindowRect(handle, &rect);
  monitor = MonitorFromRect(&rect, MONITOR_DEFAULTTONEAREST);
  mi.cbSize = sizeof mi;
  GetMonitorInfo(monitor, &mi);
  return mi;
}

bool IsWindowsXP() {
  OSVERSIONINFO osversion;
  ZeroMemory(&osversion, sizeof osversion);
  osversion.dwOSVersionInfoSize = sizeof osversion;
  GetVersionEx(&osversion);
  return osversion.dwMajorVersion < 6;
}

bool IsAeroGlassEnabled() {
  if (IsWindowsXP()) {
    return false;
  }

  BOOL enabled = FALSE;
  return SUCCEEDED(DwmIsCompositionEnabled(&enabled)) && enabled;
}

void SetAeroGlass(HWND handle, bool enable_on_maximized) {
  if (!IsAeroGlassEnabled()) {
    return;
  }
  MARGINS margin = { -1, -1, -1, -1 };
  DwmExtendFrameIntoClientArea(handle, &margin);
  if (enable_on_maximized) {
    DWM_BLURBEHIND blurBehind;
    blurBehind.dwFlags = DWM_BB_ENABLE | DWM_BB_TRANSITIONONMAXIMIZED;
    blurBehind.fEnable = true;
    blurBehind.fTransitionOnMaximized = true;
    DwmEnableBlurBehindWindow(handle, &blurBehind);
  }
}

void RunJavaScriptCode(CefRefPtr<CefBrowser> browser, CefString code) {
  if (browser.get()) {
    browser->GetMainFrame()->ExecuteJavaScript(code, "about:blank", 0);
  }
}

}  // hex
