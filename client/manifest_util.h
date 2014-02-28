// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#ifndef HEX_CLIENT_MANIFEST_UTIL_H_
#define HEX_CLIENT_MANIFEST_UTIL_H_

#include "json/json.h"
#include "hex_shared.h"
#include "util.h"

namespace hexclient {

using namespace hex;

class ManifestUtil {
public:
  static ManifestUtil& Instance() {
    static ManifestUtil manifest;
    return manifest;
  }
  bool ReadManifest();
  bool WriteManifest();
  bool Parse();
  bool UpdateWithJson(std::string json_string);
  std::string ErrorMessages() { return error_messages_; }
  std::wstring StyledString();

  std::string FirstPage() { return first_page_; }
  std::string ApplicationTitle() { return application_title_; }
  std::string Version() { return version_; }
  std::string Locale() { return locale_; }
  bool MultipleProcess() { return multiple_process_; }
  std::string UserAgent() { return user_agent_; }
  bool LaunchNodeInAllPages() { return launch_node_in_all_pages_; }
  bool LaunchNodeInBackground() { return launch_node_in_background_; }
  bool LoadNodeManually() { return load_node_manually_; }
  bool DisableAsyncNodeApis() { return disable_async_node_apis_; }
  std::string CachePath() { return cache_path_; }
  int RemoteDebuggingPort() { return remote_debugging_port_; }
  bool DisableDebugLog() { return disable_debug_log_; }
  bool QuitAfterMainWindowClosed() { return quit_after_main_window_closed_; }
  std::string NpapiPluginDirectory() { return npapi_plugin_directory_; }
  bool DisableIMEComposition() { return disable_ime_composition_; }

  FormStyleType FormStyle() { return form_style_; }
  bool FormPlain() { return form_plain_; }
  bool FormSystemButtons() { return form_system_buttons_; }
  int FormTransmissionColor() { return form_transmission_color_; }
  bool FormTransparentBrowser() { return form_transparent_browser_; }
  bool FormAdjustedTransparentBrowser() {
    return form_adjusted_transparent_browser_;
  }
  bool FormFixed() { return form_fixed_; }
  bool FormDisableFormApis() { return form_disable_form_apis_; }
  int FormOpacity() { return form_opacity_; }
  bool FormHookSystemCommand() { return form_hook_system_command_; }
  hex::FormState FormLaunchState() { return form_launch_state_; }
  int FormLaunchWidth() { return form_launch_width_; }
  int FormLaunchHeight() { return form_launch_height_; }
  int FormLaunchX() { return form_launch_x_; }
  int FormLaunchY() { return form_launch_y_; }
  int FormMinWidth() { return form_min_width_; }
  int FormMinHeight() { return form_min_height_; }
  int FormMaxWidth() { return form_max_width_; }
  int FormMaxHeight() { return form_max_height_; }
  int FormBorderWidth() { return form_border_width_; }

  bool BrowserNoProxyServer() { return browser_no_proxy_server_; }
  bool BrowserWinHttpProxyResolver() { return browser_winhttp_proxy_resolver_; }
  bool BrowserDisableGpu() { return browser_disable_gpu_; }
  bool BrowserDisable3dApis() { return browser_disable_3d_apis_; }
  bool BrowserDisableDatabases() { return browser_disable_databases_; }
  bool BrowserDisableExperimentalWebgl() {
    return browser_disable_experimental_webgl_;
  }
  bool BrowserDisableFileSystem() { return browser_disable_file_system_; }
  bool BrowserDisableGeolocation() { return browser_disable_geolocation_; }
  bool BrowserDisableGpuProcessPrelaunch() {
    return browser_disable_gpu_process_prelaunch_;
  }
  bool BrowserDisableJava() { return browser_disable_java_; }
  bool BrowserDisableJavascript() { return browser_disable_javascript_; }
  bool BrowserDisableJavascriptI18nApi() {
    return browser_disable_javascript_i18n_api_;
  }
  bool BrowserDisableLocalStorage() { return browser_disable_local_storage_; }
  bool BrowserDisableLogging() { return browser_disable_logging_; }
  bool BrowserDisablePlugins() { return browser_disable_plugins_; }
  bool BrowserDisableRendererAccessibility() {
    return browser_disable_renderer_accessibility_;
  }
  bool BrowserDisableSessionStorage() {
    return browser_disable_session_storage_;
  }
  bool BrowserDisableSpeechInput() { return browser_disable_speech_input_; }
  bool BrowserDisableWebSockets() { return browser_disable_web_sockets_; }
  bool BrowserInProcessGpu() { return browser_in_process_gpu_; }
  bool BrowserInProcessPlugins() { return browser_in_process_plugins_; }
  bool BrowserWebSecurityDisabled() { return browser_web_security_disabled_; }
  bool BrowserFileAccessFromFileUrlsAllowed() {
    return browser_file_access_from_file_urls_allowed_;
  }
  bool BrowserUniversalAccessFromFileUrlsAllowed() {
    return browser_universal_access_from_file_urls_allowed_;
  }

protected:
  ManifestUtil();
  ~ManifestUtil();

  std::wstring file_name_;
  std::string json_content_;
  std::string error_messages_;
  Json::Value root_;
  std::string first_page_;
  std::string application_title_;
  std::string version_;
  std::string locale_;
  bool multiple_process_;
  std::string user_agent_;
  bool launch_node_in_all_pages_;
  bool launch_node_in_background_;
  bool load_node_manually_;
  bool disable_async_node_apis_;
  std::string cache_path_;
  int remote_debugging_port_;
  bool disable_debug_log_;
  bool quit_after_main_window_closed_;
  std::string npapi_plugin_directory_;
  bool disable_ime_composition_;

  FormStyleType form_style_;
  bool form_plain_;
  bool form_system_buttons_;
  int form_transmission_color_;
  bool form_transparent_browser_;
  bool form_adjusted_transparent_browser_;
  bool form_fixed_;
  bool form_disable_form_apis_;
  int form_opacity_;
  bool form_hook_system_command_;
  hex::FormState form_launch_state_;
  int form_launch_width_;
  int form_launch_height_;
  int form_launch_x_;
  int form_launch_y_;
  int form_min_width_;
  int form_min_height_;
  int form_max_width_;
  int form_max_height_;
  int form_border_width_;

  bool browser_no_proxy_server_;
  bool browser_winhttp_proxy_resolver_;
  bool browser_disable_gpu_;
  bool browser_disable_3d_apis_;
  bool browser_disable_databases_;
  bool browser_disable_experimental_webgl_;
  bool browser_disable_file_system_;
  bool browser_disable_geolocation_;
  bool browser_disable_gpu_process_prelaunch_;
  bool browser_disable_java_;
  bool browser_disable_javascript_;
  bool browser_disable_javascript_i18n_api_;
  bool browser_disable_local_storage_;
  bool browser_disable_logging_;
  bool browser_disable_plugins_;
  bool browser_disable_renderer_accessibility_;
  bool browser_disable_session_storage_;
  bool browser_disable_speech_input_;
  bool browser_disable_web_sockets_;
  bool browser_in_process_gpu_;
  bool browser_in_process_plugins_;
  bool browser_web_security_disabled_;
  bool browser_file_access_from_file_urls_allowed_;
  bool browser_universal_access_from_file_urls_allowed_;
};

}  // namespace hexclient

#endif  // HEX_CLIENT_MANIFEST_UTIL_H_
