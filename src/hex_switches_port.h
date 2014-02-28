// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#ifndef HEX_SRC_HEX_SWITCHES_PORT_H_
#define HEX_SRC_HEX_SWITCHES_PORT_H_

#include "hex_export.h"

namespace hex {

HEX_EXPORT void SetHeXModule(bool hex_module);
HEX_EXPORT bool HeXModule();
#if defined(OS_WIN)
//HEX_EXPORT void SetDesktopWidget(bool into_desktop);
//HEX_EXPORT bool DesktopWidget();
#endif  // OS_WIN
HEX_EXPORT void SetSingleProcess(bool single_process);
HEX_EXPORT bool SingleProcess();
HEX_EXPORT void SetDisableFormApis(bool apis);
HEX_EXPORT bool DisableFormApis();
HEX_EXPORT void SetLockedPath(const char* path);
HEX_EXPORT const char* LockedPath();
HEX_EXPORT void SetDisabledNodeModules(const char* disabled);
HEX_EXPORT const char* DisabledNodeModules();
HEX_EXPORT void SetEnabledNodeModules(const char* enabled);
HEX_EXPORT const char* EnabledNodeModules();
HEX_EXPORT void SetDisableNodeThirdPartyNativeModules(bool no_native);
HEX_EXPORT bool DisableNodeThirdPartyNativeModules();
HEX_EXPORT void SetNodeArgs(const char* args);
HEX_EXPORT const char* NodeArgs();
HEX_EXPORT void SetLaunchNodeInAllPages(bool in_all_pages);
HEX_EXPORT bool LaunchNodeInAllPages();
HEX_EXPORT void SetLaunchNodeInBackground(bool in_background);
HEX_EXPORT bool LaunchNodeInBackground();
HEX_EXPORT void SetLoadNodeManually(bool manually);
HEX_EXPORT bool LoadNodeManually();
HEX_EXPORT void SetDisableAsyncNodeApis(bool async);
HEX_EXPORT bool DisableAsyncNodeApis();

}  // namespace hex

#endif  // HEX_SRC_HEX_SWITCHES_PORT_H_
