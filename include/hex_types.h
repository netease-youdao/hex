// Copyright (c) 2012 Youdao. All rights reserved. Use of this source code is
// governed by a BSD-style license that can be found in the LICENSE file.

#ifndef HEX_SRC_HEX_TYPES_H
#define HEX_SRC_HEX_TYPES_H

#include "include/internal/cef_string.h"

#ifdef __cplusplus
extern "C" {
#endif

///
// Initialization heX settings. Specify NULL or 0 to get the recommended default
// values.
///
typedef struct _hex_settings_t {
  ///
  // Size of this structure.
  ///
  size_t size;

  ///
  // The arguments of Node.JS.
  ///
  cef_string_t node_args;

  ///
  // The list of Node.JS' modules we want to disable separating by comma.
  ///
  cef_string_t disable_node_modules;

  ///
  // The list of Node.JS' modules we want to enable separating by comma. This
  // list has a higher priority than disabled_nodejs_modules.
  ///
  cef_string_t enable_node_modules;

  ///
  // The path to be locked in Node.JS.
  ///
  cef_string_t locked_path;

  ///
  // Set to true (1) to disable the third-party native modules in Node.JS.
  ///
  bool disable_node_third_party_native_modules;

  ///
  // Set to true (1) to launch Node.JS in all browser pages, including the popup
  // pages, otherwise Node.JS will be launched only in main page.
  ///
  bool launch_node_in_all_pages;

  ///
  // Set to true (1) to launch Node.JS in off-screen rendering background page.
  ///
  bool launch_node_in_background;

  ///
  // Set to true (1) to load Node.JS manually when you want to use.
  ///
  bool load_node_manually;

  ///
  // Set to true (1) to disable the event loop in Node, so all async APIs of
  // Node will not be available.
  ///
  bool disable_async_node_apis;

  ///
  // Set to true (1) to quit the application when the main window is closed.
  ///
  bool quit_after_main_window_closed;

  ///
  // Set to true (1) to indicate that this is not a stand-alone heX but a
  // module, and the browser/widget/renderer window will not be subclassed in
  // Windows.
  ///
  bool hex_module;

  ///
  // The imaginary border width of the captionless form.
  ///
  int border_width;

  ///
  // The style of the form.
  ///
  int form_style;

  ///
  // Set to true (1) to create a transparent browser.
  ///
  bool transparent_browser;

  ///
  // The color that can penetrate the window. -1 indicates NONE, -2 indicates
  // FULL.
  ///
  int transmission_color;

  ///
  // Set to true (1) to enable system buttons in transparent browser mode.
  ///
  bool system_buttons;

  ///
  // Set to true (1) to disable all the form API.
  ///
  bool disable_form_apis;

  ///
  // The opacity of the form, set to NONE (-1) to disable the opacity.
  ///
  int opacity;

  ///
  // The title of the application.
  ///
  cef_string_t application_title;

  ///
  // Set to true (1) to hook the system command of the form, and
  // "beforeSystemCommand" event will be enabled.
  ///
  bool hook_system_command;

  ///
  // Set to true (1) to disable the IME composition window when inputing.
  ///
  bool disable_ime_composition;

  ///
  // Set to true (1) to make the form grotesquely.
  ///
  bool grotesque;

  /// --- Chrome content switches --- ///
  ///
  // Set to true (1) to disable proxy server and always make direct connections.
  ///
  bool no_proxy_server;

  ///
  // Use WinHTTP to fetch and evaluate PAC scripts. Otherwise the default is
  // to use Chromium's network stack to fetch, and V8 to evaluate.
  ///
  bool winhttp_proxy_resolver;

  ///
  // Disables GPU hardware acceleration.
  ///
  bool disable_gpu;

  ///
  // Disables client-visible 3D APIs, in particular WebGL and Pepper 3D.
  // This is controlled by policy and is kept separate from the other
  // enable/disable switches to avoid accidentally regressing the policy
  // support for controlling access to these APIs.
  ///
  bool disable_3d_apis;

  ///
  // Disables HTML5 DB support.
  ///
  bool disable_databases;

  ///
  // Disable experimental WebGL support.
  ///
  bool disable_experimental_webgl;

  ///
  // Disable FileSystem API.
  ///
  bool disable_file_system;

  ///
  // Suppresses support for the Geolocation javascript API.
  ///
  bool disable_geolocation;

  ///
  // Do not launch the GPU process shortly after browser process launch. Instead
  // launch it when it is first needed.
  ///
  bool disable_gpu_process_prelaunch;

  ///
  // Prevent Java from running.
  ///
  bool disable_java;

  ///
  // Don't execute JavaScript (browser JS like the new tab page still runs).
  ///
  bool disable_javascript;

  ///
  // Disable JavaScript I18N API.
  ///
  bool disable_javascript_i18n_api;

  ///
  // Disable LocalStorage.
  ///
  bool disable_local_storage;

  ///
  // Force logging to be disabled.  Logging is enabled by default in debug
  // builds.
  ///
  bool disable_logging;

  ///
  // Prevent plugins from running.
  ///
  bool disable_plugins;

  ///
  // Turns off the accessibility in the renderer.
  ///
  bool disable_renderer_accessibility;

  ///
  // Disable session storage.
  ///
  bool disable_session_storage;

  ///
  // Disables speech input.
  ///
  bool disable_speech_input;

  ///
  // Run the GPU process as a thread in the browser process.
  ///
  bool in_process_gpu;

  ///
  // Runs plugins inside the renderer process.
  ///
  bool in_process_plugins;

  ///
  // Disable Web Sockets support.
  ///
  bool disable_web_sockets;
} hex_settings_t;

#ifdef __cplusplus
}
#endif

#endif  // HEX_SRC_HEX_TYPES_H
