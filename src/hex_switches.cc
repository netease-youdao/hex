// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "hex_switches.h"

namespace switches {

#if defined(HEXCLIENT)
// This switch indicates that heX is in module mode.
const char kHeXModule[]                 = "hex-module";
#if defined(OS_WIN)
// Embed heX's window into the desktop, like a widget.
//const char kDesktopWidget[]             = "desktop-widget";
// Disable the IME composition window when inputing.
const char kDisableIMEComposition[]     = "disable-ime-composition";
#endif  // OS_WIN
// Disable window APIs in heX.
const char kDisableFormApis[]           = "disable-form-apis";
// The arguments of Node.JS.
const char kNodeArgs[]                  = "node-args";
// Disable the event loop in Node, so all async APIs of Node will not be
// available.
const char kDisableAsyncNodeApis[]      = "disable-async-node-apis";
// The list of Node.JS' modules we want to disable separating by comma.
const char kDisabledNodeModules[]       = "disabled-node-modules";
// The list of Node.JS' modules we want to enable separating by comma.
const char kEnabledNodeModules[]        = "enabled-node-modules";
// The path to be locked in Node.JS.
const char kLockedPath[]                = "locked-path";
// Disable the third-party native modules in Node.JS.
const char kDisableNodeThirdPartyNativeModules[]
    = "disable-node-third-party-native-modules";
// Use the Node's typed-arrays instead of Chromium's.
const char kUseNodeTypedArrays[]        = "use-node-typed-arrays";
// Launch Node.JS in all browser pages.
const char kLaunchNodeInAllPages[]      = "launch-node-in-all-pages";
// Load Node.JS manually when you want to use.
const char kLoadNodeManually[]          = "load-node-manually";
// Quit the application directly when the main window is closed.
const char kQuitAfterMainWindowClosed[] = "quit-after-main-window-closed";
#endif  // HEXCLIENT

}  // namespace switches
