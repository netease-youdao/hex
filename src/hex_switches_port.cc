// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "hex_switches_port.h"
#include "default_values.h"
#include <string>

namespace hex {

static bool hex_module_ = false;
#if defined(OS_WIN)
static bool desktop_widget_ = false;
#endif  // OS_WIN
static bool single_process_ = !GET_DEFAULT_VALUE(multiple_process);
static std::string locked_path_ = GET_DEFAULT_VALUE(locked_path);
static std::string disabled_node_modules_ =
    GET_DEFAULT_VALUE(disabled_node_modules);
static std::string enabled_node_modules_ =
    GET_DEFAULT_VALUE(enabled_node_modules);
static bool disable_node_third_party_native_modules_ =
    GET_DEFAULT_VALUE(disable_node_third_party_native_modules);
static std::string node_args_ = EMPTY_STRING;
static bool launch_node_in_all_pages_ =
    GET_DEFAULT_VALUE(launch_node_in_all_pages);
static bool launch_node_in_background_ =
    GET_DEFAULT_VALUE(launch_node_in_background);
static bool load_node_manually_ = GET_DEFAULT_VALUE(load_node_manually);
static bool disable_async_node_apis_ =
    GET_DEFAULT_VALUE(disable_async_node_apis);
static bool disable_form_apis_ = GET_DEFAULT_VALUE(form_disable_form_apis);

void SetHeXModule(bool hex_module) {
  hex_module_ = hex_module;
}

bool HeXModule() {
  return hex_module_;
}

#if defined(OS_WIN)
/*void SetDesktopWidget(bool desktop_widget) {
  desktop_widget_ = desktop_widget;
}

bool DesktopWidget() {
  return desktop_widget_;
}*/
#endif  // OS_WIN

void SetSingleProcess(bool single_process) {
  single_process_ = single_process;
}

bool SingleProcess() {
  return single_process_;
}

void SetDisableFormApis(bool apis) {
  disable_form_apis_ = apis;
}

bool DisableFormApis() {
  return disable_form_apis_;
}

void SetLockedPath(const char* path) {
  locked_path_ = path;
}

const char* LockedPath() {
  return locked_path_.c_str();
}

void SetDisabledNodeModules(const char* disabled) {
  disabled_node_modules_ = disabled;
}

const char* DisabledNodeModules() {
  return disabled_node_modules_.c_str();
}

void SetEnabledNodeModules(const char* enabled) {
  enabled_node_modules_ = enabled;
}

const char* EnabledNodeModules() {
  return enabled_node_modules_.c_str();
}

void SetDisableNodeThirdPartyNativeModules(bool no_native) {
  disable_node_third_party_native_modules_ = no_native;
}

bool DisableNodeThirdPartyNativeModules() {
  return disable_node_third_party_native_modules_;
}

void SetNodeArgs(const char* args) {
  node_args_ = args;
}

const char* NodeArgs() {
  return node_args_.c_str();
}

void SetLaunchNodeInAllPages(bool in_all_pages) {
  launch_node_in_all_pages_ = in_all_pages;
}

bool LaunchNodeInAllPages() {
  return launch_node_in_all_pages_;
}

void SetLaunchNodeInBackground(bool in_background) {
  launch_node_in_background_ = in_background;
}

bool LaunchNodeInBackground() {
  return launch_node_in_background_;
}

void SetLoadNodeManually(bool manually) {
  load_node_manually_ = manually;
}

bool LoadNodeManually() {
  return load_node_manually_;
}

void SetDisableAsyncNodeApis(bool async) {
  disable_async_node_apis_ = async;
}

bool DisableAsyncNodeApis() {
  return disable_async_node_apis_;
}

}  // namespace hex
