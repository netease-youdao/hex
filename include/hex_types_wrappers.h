// Copyright (c) 2012 Youdao. All rights reserved. Use of this source code is
// governed by a BSD-style license that can be found in the LICENSE file.

#ifndef HEX_SRC_HEX_TYPES_WRAPPERS_H
#define HEX_SRC_HEX_TYPES_WRAPPERS_H

#include "include/internal/cef_string.h"
#include "include/internal/cef_types.h"
#include "hex_types.h"

struct HeXSettingsTraits {
  typedef hex_settings_t struct_type;

  static inline void init(struct_type* s) {
    s->size = sizeof(struct_type);
  }

  static inline void clear(struct_type* s) {
    cef_string_clear(&s->node_args);
    cef_string_clear(&s->disable_node_modules);
    cef_string_clear(&s->enable_node_modules);
    cef_string_clear(&s->locked_path);
    cef_string_clear(&s->application_title);
  }

  static inline void set(const struct_type* src, struct_type* target,
      bool copy) {
    cef_string_set(src->node_args.str,
                   src->node_args.length,
                   &target->node_args, copy);
    cef_string_set(src->disable_node_modules.str,
                   src->disable_node_modules.length,
                   &target->disable_node_modules, copy);
    cef_string_set(src->enable_node_modules.str,
                   src->enable_node_modules.length,
                   &target->enable_node_modules, copy);
    cef_string_set(src->locked_path.str, src->locked_path.length,
                   &target->locked_path, copy);
    target->disable_node_third_party_native_modules =
        src->disable_node_third_party_native_modules;
    target->launch_node_in_all_pages = src->launch_node_in_all_pages;
    target->launch_node_in_background = src->launch_node_in_background;
    target->load_node_manually = src->load_node_manually;
    target->disable_async_node_apis = src->disable_async_node_apis;
    target->quit_after_main_window_closed = src->quit_after_main_window_closed;
    target->hex_module = src->hex_module;
    target->border_width = src->border_width;
    target->form_style = src->form_style;
    target->transparent_browser = src->transparent_browser;
    target->transmission_color = src->transmission_color;
    target->system_buttons = src->system_buttons;
    target->disable_form_apis = src->disable_form_apis;
    target->opacity = src->opacity;
    cef_string_set(src->application_title.str, src->application_title.length,
        &target->application_title, copy);
    target->hook_system_command = src->hook_system_command;
    target->disable_ime_composition = src->disable_ime_composition;
    target->grotesque = src->grotesque;
    target->no_proxy_server = src->no_proxy_server;
    target->winhttp_proxy_resolver = src->winhttp_proxy_resolver;
    target->disable_gpu = src->disable_gpu;
    target->disable_3d_apis = src->disable_3d_apis;
    target->disable_databases = src->disable_databases;
    target->disable_experimental_webgl = src->disable_experimental_webgl;
    target->disable_file_system = src->disable_file_system;
    target->disable_geolocation = src->disable_geolocation;
    target->disable_gpu_process_prelaunch = src->disable_gpu_process_prelaunch;
    target->disable_java = src->disable_java;
    target->disable_javascript = src->disable_javascript;
    target->disable_javascript_i18n_api = src->disable_javascript_i18n_api;
    target->disable_local_storage = src->disable_local_storage;
    target->disable_logging = src->disable_logging;
    target->disable_plugins = src->disable_plugins;
    target->disable_renderer_accessibility =
        src->disable_renderer_accessibility;
    target->disable_session_storage = src->disable_session_storage;
    target->disable_speech_input = src->disable_speech_input;
    target->disable_web_sockets = src->disable_web_sockets;
    target->in_process_gpu = src->in_process_gpu;
    target->in_process_plugins = src->in_process_plugins;
  }
};

///
// Class representing initialization settings.
///
typedef CefStructBase<HeXSettingsTraits> HeXSettings;

#endif  // HEX_SRC_HEX_TYPES_WRAPPERS_H
