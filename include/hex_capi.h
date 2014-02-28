// Copyright (c) 2012 Youdao. All rights reserved. Use of this source code is
// governed by a BSD-style license that can be found in the LICENSE file.

#ifndef HEX_INCLUDE_HEX_CAPI_H_
#define HEX_INCLUDE_HEX_CAPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "include/capi/cef_base_capi.h"

///
// This function should be called after CefInitialize() on the main application
// thread to initialize heX settings.
///
CEF_EXPORT int hex_settings_initialize(const struct _hex_settings_t* settings);

#ifdef __cplusplus
}
#endif

#endif  // HEX_INCLUDE_HEX_CAPI_H_
