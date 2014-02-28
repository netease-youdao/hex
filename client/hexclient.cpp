// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "hexclient.h"
#include <stdio.h>
#include <cstdlib>
#include <sstream>
#include <string>
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "include/cef_web_plugin.h"
#include "client_handler.h"
#include "client_switches.h"
#include "string_util.h"
#include "util.h"
#ifdef WIN32
#include <atlconv.h>
#endif  // WIN32
#include "hex_switches_port.h"
#include "hex_use_node.h"

CefRefPtr<ClientHandler> g_handler;
CefRefPtr<CefCommandLine> g_command_line;

CefRefPtr<CefBrowser> AppGetBrowser() {
  if (!g_handler.get())
    return NULL;
  return g_handler->GetBrowser();
}

namespace hexclient {

CefWindowHandle AppGetMainHwnd() {
  if (!g_handler.get())
    return NULL;
  return g_handler->GetMainHwnd();
}

void AppGetSettingsFromManifest(CefSettings& settings,
                                HeXSettings& hex_settings,
                                CefRefPtr<ClientApp> app,
                                ManifestUtil& manifest) {
  ASSERT(app.get());

#if defined(OS_WIN)
  USES_CONVERSION;
#else
  #define A2W(E) E
#endif  // OS_WIN
#if !defined(NO_DEVTOOLS)
  if (manifest.RemoteDebuggingPort() >= 0 &&
      manifest.RemoteDebuggingPort() <= 65535  &&
      !IsPortUsed(manifest.RemoteDebuggingPort())) {
    settings.remote_debugging_port = manifest.RemoteDebuggingPort();
  }
#endif  // NO_DEVTOOLS
  CefString(&settings.cache_path) = A2W(manifest.CachePath().c_str());
  settings.single_process = !manifest.MultipleProcess();
  hex::SetSingleProcess(settings.single_process);
  if (!manifest.Locale().empty()) {
    CefString(&settings.locale) = A2W(manifest.Locale().c_str());
  } else {
    settings.pack_loading_disabled = true;
  }
  if (!manifest.UserAgent().empty())
    CefString(&settings.user_agent) = A2W(manifest.UserAgent().c_str());
  if (manifest.DisableDebugLog()) {
    settings.log_severity = LOGSEVERITY_DISABLE;
  }

  if (hex::CanUseNode()) {
    hex_settings.launch_node_in_all_pages = manifest.LaunchNodeInAllPages();
    hex::SetLaunchNodeInAllPages(hex_settings.launch_node_in_all_pages);
    hex_settings.launch_node_in_background = manifest.LaunchNodeInBackground();
    hex::SetLaunchNodeInBackground(hex_settings.launch_node_in_background);
    hex_settings.load_node_manually = manifest.LoadNodeManually();
    hex::SetLoadNodeManually(hex_settings.load_node_manually);
    hex_settings.disable_async_node_apis = manifest.DisableAsyncNodeApis();
    hex::SetDisableAsyncNodeApis(hex_settings.disable_async_node_apis);
  }
  hex_settings.disable_ime_composition = manifest.DisableIMEComposition();

  hex_settings.border_width = manifest.FormBorderWidth();
  hex_settings.form_style = static_cast<int>(manifest.FormStyle());
  hex_settings.transparent_browser = manifest.FormTransparentBrowser();
  hex_settings.transmission_color = manifest.FormTransmissionColor();
  hex_settings.system_buttons = manifest.FormSystemButtons();
  hex_settings.disable_form_apis = manifest.FormDisableFormApis();
  hex::SetDisableFormApis(hex_settings.disable_form_apis);
  hex_settings.opacity = manifest.FormOpacity();

  CefString(&hex_settings.application_title) =
      A2W(manifest.ApplicationTitle().c_str());

  hex_settings.no_proxy_server = manifest.BrowserNoProxyServer();
  hex_settings.winhttp_proxy_resolver = manifest.BrowserWinHttpProxyResolver();
  hex_settings.disable_gpu = manifest.BrowserDisableGpu();
  hex_settings.disable_3d_apis = manifest.BrowserDisable3dApis();
  hex_settings.disable_databases = manifest.BrowserDisableDatabases();
  hex_settings.disable_experimental_webgl =
      manifest.BrowserDisableExperimentalWebgl();
  hex_settings.disable_file_system = manifest.BrowserDisableFileSystem();
  hex_settings.disable_geolocation = manifest.BrowserDisableGeolocation();
  hex_settings.disable_gpu_process_prelaunch =
      manifest.BrowserDisableGpuProcessPrelaunch();
  hex_settings.disable_java = manifest.BrowserDisableJava();
  hex_settings.disable_javascript = manifest.BrowserDisableJavascript();
  hex_settings.disable_javascript_i18n_api =
      manifest.BrowserDisableJavascriptI18nApi();
  hex_settings.disable_local_storage = manifest.BrowserDisableLocalStorage();
  hex_settings.disable_logging = manifest.BrowserDisableLogging();
  hex_settings.disable_plugins = manifest.BrowserDisablePlugins();
  hex_settings.disable_renderer_accessibility =
      manifest.BrowserDisableRendererAccessibility();
  hex_settings.disable_session_storage =
      manifest.BrowserDisableSessionStorage();
  hex_settings.disable_speech_input = manifest.BrowserDisableSpeechInput();
  hex_settings.disable_web_sockets = manifest.BrowserDisableWebSockets();
}

void AppGetBrowserSettingsFromManifest(CefBrowserSettings& settings,
                                       ManifestUtil& manifest) {
  settings.web_security =
      manifest.BrowserWebSecurityDisabled() ? STATE_DISABLED : STATE_ENABLED;
  settings.file_access_from_file_urls =
      manifest.BrowserFileAccessFromFileUrlsAllowed() ? STATE_ENABLED
                                                      : STATE_DISABLED;
  settings.universal_access_from_file_urls =
      manifest.BrowserUniversalAccessFromFileUrlsAllowed() ? STATE_ENABLED
                                                           : STATE_DISABLED;
}

}  // namespace hexclient
