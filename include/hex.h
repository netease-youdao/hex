// Copyright (c) 2012 Youdao. All rights reserved. Use of this source code is
// governed by a BSD-style license that can be found in the LICENSE file.

#ifndef HEX_INCLUDE_HEX_H_
#define HEX_INCLUDE_HEX_H_

#include "hex_types.h"

///
// This function should be called after CefInitialize() on the main application
// thread to initialize heX settings.
///
/*--cef()--*/
bool HeXSettingsInitialize(const HeXSettings& settings);

#endif  // HEX_INCLUDE_HEX_H_
