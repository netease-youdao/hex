// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#ifndef HEX_SRC_DEFAULT_VALUES_H_
#define HEX_SRC_DEFAULT_VALUES_H_

namespace hex {

#define SYSTEM_DEFAULT SYSTEMDEFAULT
#define EMPTY_STRING ""

#define DEF_first_page_VAL                          "chrome://version"
#define DEF_application_title_VAL                   "heX"
#define DEF_version_VAL                             EMPTY_STRING
#define DEF_locale_VAL                              EMPTY_STRING
#define DEF_user_agent_VAL                          EMPTY_STRING
#define DEF_multiple_process_VAL                    false
#define DEF_launch_node_in_all_pages_VAL            false
#define DEF_launch_node_in_background_VAL           false
#define DEF_load_node_manually_VAL                  false
#define DEF_disable_async_node_apis_VAL             false
#define DEF_cache_path_VAL                          EMPTY_STRING
#define DEF_remote_debugging_port_VAL               0
#define DEF_disable_debug_log_VAL                   false
#define DEF_quit_after_main_window_closed_VAL       false
#define DEF_npapi_plugin_directory_VAL              EMPTY_STRING
#define DEF_disable_ime_composition_VAL             false

#define DEF_form_style_VAL                          STANDARD
#define DEF_form_plain_VAL                          false
#define DEF_form_system_buttons_VAL                 false
#define DEF_form_transmission_color_VAL             NONE
#define DEF_form_transparent_browser_VAL            false
#define DEF_form_fixed_VAL                          false
#define DEF_form_disable_form_apis_VAL              false
#define DEF_form_opacity_VAL                        NONE
#define DEF_form_hook_system_command_VAL            false
#define DEF_form_launch_state_VAL                   NORMAL
#define DEF_form_launch_width_VAL                   SYSTEM_DEFAULT
#define DEF_form_launch_height_VAL                  SYSTEM_DEFAULT
#define DEF_form_launch_x_VAL                       SYSTEM_DEFAULT
#define DEF_form_launch_y_VAL                       SYSTEM_DEFAULT
#define DEF_form_min_width_VAL                      0
#define DEF_form_min_height_VAL                     0
#define DEF_form_max_width_VAL                      0
#define DEF_form_max_height_VAL                     0
#define DEF_form_border_width_VAL                   5

#define DEF_browser_no_proxy_server_VAL                         false
#define DEF_browser_winhttp_proxy_resolver_VAL                  false
#define DEF_browser_disable_gpu_VAL                             false
#define DEF_browser_disable_3d_apis_VAL                         false
#define DEF_browser_disable_databases_VAL                       false
#define DEF_browser_disable_experimental_webgl_VAL              false
#define DEF_browser_disable_file_system_VAL                     false
#define DEF_browser_disable_geolocation_VAL                     false
#define DEF_browser_disable_gpu_process_prelaunch_VAL           false
#define DEF_browser_disable_java_VAL                            false
#define DEF_browser_disable_javascript_VAL                      false
#define DEF_browser_disable_javascript_i18n_api_VAL             false
#define DEF_browser_disable_local_storage_VAL                   false
#define DEF_browser_disable_logging_VAL                         false
#define DEF_browser_disable_plugins_VAL                         false
#define DEF_browser_disable_renderer_accessibility_VAL          false
#define DEF_browser_disable_session_storage_VAL                 false
#define DEF_browser_disable_speech_input_VAL                    false
#define DEF_browser_disable_web_sockets_VAL                     false
#define DEF_browser_in_process_gpu_VAL                          false
#define DEF_browser_in_process_plugins_VAL                      false
#define DEF_browser_web_security_disabled_VAL                   false
#define DEF_browser_file_access_from_file_urls_allowed_VAL      false
#define DEF_browser_universal_access_from_file_urls_allowed_VAL false

#define DEF_locked_path_VAL                                     EMPTY_STRING
#define DEF_disabled_node_modules_VAL                           EMPTY_STRING
#define DEF_enabled_node_modules_VAL                            EMPTY_STRING
#define DEF_disable_node_third_party_native_modules_VAL         false

#define GET_DEFAULT_VALUE(key) DEF_##key##_VAL

}  // namespace hex

#endif  // HEX_SRC_DEFAULT_VALUES_H_
