// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "include/cef_app.h"
#include "include/capi/cef_app_capi.h"
#include "include/cef_version.h"
#include "libcef_dll/cef_logging.h"
#include "hex/include/hex.h"
#include "hex/include/hex_capi.h"

// Define used to facilitate parsing.
#define CEF_GLOBAL

CEF_GLOBAL bool HeXSettingsInitialize(const HeXSettings& settings) {
  const char* api_hash = cef_api_hash(0);
  if (strcmp(api_hash, CEF_API_HASH_PLATFORM)) {
    // The libcef API hash does not match the current header API hash.
    DCHECK(false);
    return false;
  }

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Unverified params: application

  // Execute
  int _retval = hex_settings_initialize(
    &settings);

  // Return type: bool
  return _retval?true:false;
}
