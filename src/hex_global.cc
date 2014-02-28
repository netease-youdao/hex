// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "libcef/browser/context.h"
#include "base/logging.h"
#include "include/cef_base.h"

bool HeXSettingsInitialize(const HeXSettings& settings) {
  // Return true if the global context already exists.
  if (!CefContext::Get())
    return false;

  if (settings.size != sizeof(hex_settings_t)) {
    NOTREACHED() << "invalid HeXSettings structure size";
    return false;
  }

  CefContext::Get()->SetHeXSettings(settings);

  return true;
}
