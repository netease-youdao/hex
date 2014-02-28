// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#ifndef HEX_SRC_HEX_SWITCHES_H_
#define HEX_SRC_HEX_SWITCHES_H_

#include "hex_export.h"

namespace switches {

#if defined(HEXCLIENT)
HEX_EXPORT extern const char kHeXModule[];
#if defined(OS_WIN)
//HEX_EXPORT extern const char kDesktopWidget[];
HEX_EXPORT extern const char kDisableIMEComposition[];
#endif  // OS_WIN
HEX_EXPORT extern const char kDisableFormApis[];
HEX_EXPORT extern const char kNodeArgs[];
HEX_EXPORT extern const char kDisableAsyncNodeApis[];
HEX_EXPORT extern const char kDisabledNodeModules[];
HEX_EXPORT extern const char kEnabledNodeModules[];
HEX_EXPORT extern const char kLockedPath[];
HEX_EXPORT extern const char kDisableNodeThirdPartyNativeModules[];
HEX_EXPORT extern const char kUseNodeTypedArrays[];
HEX_EXPORT extern const char kLaunchNodeInAllPages[];
HEX_EXPORT extern const char kLoadNodeManually[];
HEX_EXPORT extern const char kQuitAfterMainWindowClosed[];
#endif  // HEXCLIENT

}  // namespace switches

#endif  // HEX_SRC_HEX_SWITCHES_H_
