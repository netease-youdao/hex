// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "base/bind.h"
#include "base/command_line.h"
#include "base/file_util.h"
#include "libcef/browser/context.h"
#include "libcef/common/main_delegate.h"
#include "hex/src/hex_switches.h"
#include "hex/src/hex_switches_port.h"
#include "chrome/common/chrome_switches.h"
#include "ui/surface/surface_switches.h"

void CefContext::SetHeXSettings(const HeXSettings& hex_settings) {
  if (hex_settings.size != sizeof(hex_settings_t)) {
    NOTREACHED() << "invalid HeXSettings structure size";
    return;
  }
  hex_settings_ = hex_settings;
  CommandLine* command_line = CommandLine::ForCurrentProcess();

  if (hex_settings.hex_module) {
    hex::SetHeXModule(hex_settings.hex_module);
    command_line->AppendSwitch(switches::kHeXModule);
  }

#if defined(OS_WIN)
  if (hex_settings.disable_ime_composition) {
    command_line->AppendSwitch(switches::kDisableIMEComposition);
  }
#endif  // OS_WIN

  if (settings_.single_process) {
    hex::SetSingleProcess(settings_.single_process);
  }

  if (hex_settings.node_args.length > 0) {
    hex::SetNodeArgs(CefString(&hex_settings.node_args).ToString().c_str());
    command_line->AppendSwitchNative(switches::kNodeArgs,
        CefString(&hex_settings.node_args));
  }
  if (hex_settings.disable_node_modules.length > 0) {
    hex::SetDisabledNodeModules(
        CefString(&hex_settings.disable_node_modules).ToString().c_str());
    command_line->AppendSwitchNative(switches::kDisabledNodeModules,
        CefString(&hex_settings.disable_node_modules));
  }
  if (hex_settings.enable_node_modules.length > 0) {
    hex::SetEnabledNodeModules(
        CefString(&hex_settings.enable_node_modules).ToString().c_str());
    command_line->AppendSwitchNative(switches::kEnabledNodeModules,
        CefString(&hex_settings.enable_node_modules));
  }
  if (hex_settings.locked_path.length > 0) {
    hex::SetLockedPath(
        CefString(&hex_settings.locked_path).ToString().c_str());
    command_line->AppendSwitchNative(switches::kLockedPath,
        CefString(&hex_settings.locked_path));
  }
  if (hex_settings.disable_node_third_party_native_modules) {
    hex::SetDisableNodeThirdPartyNativeModules(
        hex_settings.disable_node_third_party_native_modules);
    command_line->AppendSwitch(switches::kDisableNodeThirdPartyNativeModules);
  }
  if (hex_settings.disable_form_apis) {
    hex::SetDisableFormApis(hex_settings.disable_form_apis);
    command_line->AppendSwitch(switches::kDisableFormApis);
  }
  if (hex_settings.launch_node_in_all_pages) {
    hex::SetLaunchNodeInAllPages(hex_settings.launch_node_in_all_pages);
    command_line->AppendSwitch(switches::kLaunchNodeInAllPages);
  }
  if (hex_settings.load_node_manually) {
    hex::SetLaunchNodeInAllPages(hex_settings.load_node_manually);
    command_line->AppendSwitch(switches::kLoadNodeManually);
  }
  if (hex_settings.disable_async_node_apis) {
    hex::SetLaunchNodeInAllPages(hex_settings.disable_async_node_apis);
    command_line->AppendSwitch(switches::kDisableAsyncNodeApis);
  }
  if (hex_settings.quit_after_main_window_closed) {
    command_line->AppendSwitch(switches::kQuitAfterMainWindowClosed);
  }

  if (hex_settings.no_proxy_server) {
    CommandLine::ForCurrentProcess()->AppendSwitch(switches::kNoProxyServer);
  }
  if (hex_settings.winhttp_proxy_resolver) {
    CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kWinHttpProxyResolver);
  }
  if (hex_settings.disable_gpu) {
    CommandLine::ForCurrentProcess()->AppendSwitch(switches::kDisableGpu);
    CommandLine::ForCurrentProcess()->AppendSwitch(switches::kInProcessGPU);
  }
  if (hex_settings.disable_3d_apis) {
    CommandLine::ForCurrentProcess()->AppendSwitch(switches::kDisable3DAPIs);
  }
  if (hex_settings.disable_databases) {
    CommandLine::ForCurrentProcess()->AppendSwitch(switches::kDisableDatabases);
  }
  if (hex_settings.disable_experimental_webgl) {
    CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kDisableExperimentalWebGL);
  }
  if (hex_settings.disable_file_system) {
    CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kDisableFileSystem);
  }
  if (hex_settings.disable_geolocation) {
    CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kDisableGeolocation);
  }
  if (hex_settings.disable_gpu_process_prelaunch) {
    CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kDisableGpuProcessPrelaunch);
  }
  if (hex_settings.disable_java) {
    CommandLine::ForCurrentProcess()->AppendSwitch(switches::kDisableJava);
  }
  if (hex_settings.disable_javascript) {
    CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kDisableJavaScript);
  }
  if (hex_settings.disable_local_storage) {
    CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kDisableLocalStorage);
  }
  if (hex_settings.disable_logging) {
    CommandLine::ForCurrentProcess()->AppendSwitch(switches::kDisableLogging);
  }
  if (hex_settings.disable_plugins) {
    CommandLine::ForCurrentProcess()->AppendSwitch(switches::kDisablePlugins);
  }
  if (hex_settings.disable_renderer_accessibility) {
    CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kDisableRendererAccessibility);
  }
  if (hex_settings.disable_session_storage) {
    CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kDisableSessionStorage);
  }
  if (hex_settings.disable_speech_input) {
    CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kDisableSpeechInput);
  }
  if (hex_settings.in_process_gpu) {
    CommandLine::ForCurrentProcess()->AppendSwitch(switches::kInProcessGPU);
  }
  if (hex_settings.in_process_plugins) {
    CommandLine::ForCurrentProcess()->AppendSwitch(switches::kInProcessPlugins);
  }
}
