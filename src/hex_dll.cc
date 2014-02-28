// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "include/cef_app.h"
#include "include/capi/cef_app_capi.h"
#include "base/logging.h"
#include "hex/include/hex.h"
#include "hex/include/hex_capi.h"

CEF_EXPORT int hex_settings_initialize(const struct _hex_settings_t* settings) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: settings; type: struct_byref_const
  DCHECK(settings);
  if (!settings)
    return 0;
  // Unverified params: application

  // Translate param: settings; type: struct_byref_const
  HeXSettings settingsObj;
  if (settings)
    settingsObj.Set(*settings, false);

  // Execute
  bool _retval = HeXSettingsInitialize(
    settingsObj);

  // Return type: bool
  return _retval;
}
