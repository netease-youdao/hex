// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "manifest_util.h"
#include "default_values.h"
#include "util.h"
#include "hex_shared_win.h"
#include <algorithm>
#include <string>

namespace hexclient {

#define MANIFEST_MEMBER(key) key##_(GET_DEFAULT_VALUE(##key##))

#define MANIFEST_ROOT_RETRIEVE(key, type)                                     \
  key##_ = root_.get(#key, ##key##_).as##type##()
#define MANIFEST_FORM_RETRIEVE(key, type)                                     \
  form_##key##_ = form.get(#key, form_##key##_).as##type##()
#define MANIFEST_BROWSER_RETRIEVE(key, type)                                  \
  browser_##key##_ = browser.get(#key, browser_##key##_).as##type##()

#define MANIFEST_CORRECT_VALUE(key, value, condition) if (condition) {        \
  ##key##_ = value;\
}

#define MANIFEST_PARSE() Json::Reader reader;                                 \
Json::Features::strictMode();                                                 \
if (!reader.parse(json_content_, root_)) {                                    \
  error_messages_ = reader.getFormattedErrorMessages();                       \
  return false;                                                               \
}

ManifestUtil::ManifestUtil()
    : error_messages_(""),
      MANIFEST_MEMBER(first_page),
      MANIFEST_MEMBER(application_title),
      MANIFEST_MEMBER(version),
      MANIFEST_MEMBER(locale),
      MANIFEST_MEMBER(multiple_process),
      MANIFEST_MEMBER(user_agent),
      MANIFEST_MEMBER(launch_node_in_all_pages),
      MANIFEST_MEMBER(launch_node_in_background),
      MANIFEST_MEMBER(load_node_manually),
      MANIFEST_MEMBER(disable_async_node_apis),
      MANIFEST_MEMBER(cache_path),
      MANIFEST_MEMBER(remote_debugging_port),
      MANIFEST_MEMBER(disable_debug_log),
      MANIFEST_MEMBER(quit_after_main_window_closed),
      MANIFEST_MEMBER(npapi_plugin_directory),
      MANIFEST_MEMBER(disable_ime_composition),

      MANIFEST_MEMBER(form_style),
      MANIFEST_MEMBER(form_plain),
      MANIFEST_MEMBER(form_system_buttons),
      MANIFEST_MEMBER(form_transmission_color),
      MANIFEST_MEMBER(form_transparent_browser),
      MANIFEST_MEMBER(form_fixed),
      MANIFEST_MEMBER(form_disable_form_apis),
      MANIFEST_MEMBER(form_opacity),
      MANIFEST_MEMBER(form_hook_system_command),
      MANIFEST_MEMBER(form_launch_state),
      MANIFEST_MEMBER(form_launch_width),
      MANIFEST_MEMBER(form_launch_height),
      MANIFEST_MEMBER(form_launch_x),
      MANIFEST_MEMBER(form_launch_y),
      MANIFEST_MEMBER(form_min_width),
      MANIFEST_MEMBER(form_min_height),
      MANIFEST_MEMBER(form_max_width),
      MANIFEST_MEMBER(form_max_height),
      MANIFEST_MEMBER(form_border_width),

      MANIFEST_MEMBER(browser_no_proxy_server),
      MANIFEST_MEMBER(browser_winhttp_proxy_resolver),
      MANIFEST_MEMBER(browser_disable_gpu),
      MANIFEST_MEMBER(browser_web_security_disabled),
      MANIFEST_MEMBER(browser_disable_3d_apis),
      MANIFEST_MEMBER(browser_disable_databases),
      MANIFEST_MEMBER(browser_disable_experimental_webgl),
      MANIFEST_MEMBER(browser_disable_file_system),
      MANIFEST_MEMBER(browser_disable_geolocation),
      MANIFEST_MEMBER(browser_disable_gpu_process_prelaunch),
      MANIFEST_MEMBER(browser_disable_java),
      MANIFEST_MEMBER(browser_disable_javascript),
      MANIFEST_MEMBER(browser_disable_javascript_i18n_api),
      MANIFEST_MEMBER(browser_disable_local_storage),
      MANIFEST_MEMBER(browser_disable_logging),
      MANIFEST_MEMBER(browser_disable_plugins),
      MANIFEST_MEMBER(browser_disable_renderer_accessibility),
      MANIFEST_MEMBER(browser_disable_session_storage),
      MANIFEST_MEMBER(browser_disable_speech_input),
      MANIFEST_MEMBER(browser_disable_web_sockets),
      MANIFEST_MEMBER(browser_in_process_gpu),
      MANIFEST_MEMBER(browser_in_process_plugins),
      MANIFEST_MEMBER(browser_file_access_from_file_urls_allowed),
      MANIFEST_MEMBER(browser_universal_access_from_file_urls_allowed) {
  file_name_ = hexclient::GetFullPathInExecutionDirectory("manifest.json",
                                                          true);
}

ManifestUtil::~ManifestUtil() {
}

bool ManifestUtil::Parse() {
  MANIFEST_PARSE();

  MANIFEST_ROOT_RETRIEVE(first_page, String);
  MANIFEST_ROOT_RETRIEVE(application_title, String);
  MANIFEST_ROOT_RETRIEVE(version, String);
  MANIFEST_ROOT_RETRIEVE(locale, String);
  MANIFEST_ROOT_RETRIEVE(multiple_process, Bool);
  MANIFEST_ROOT_RETRIEVE(user_agent, String);
  MANIFEST_ROOT_RETRIEVE(launch_node_in_all_pages, Bool);
  MANIFEST_ROOT_RETRIEVE(launch_node_in_background, Bool);
  MANIFEST_ROOT_RETRIEVE(load_node_manually, Bool);
  MANIFEST_ROOT_RETRIEVE(disable_async_node_apis, Bool);
  MANIFEST_ROOT_RETRIEVE(cache_path, String);
  MANIFEST_ROOT_RETRIEVE(remote_debugging_port, Int);
  MANIFEST_ROOT_RETRIEVE(disable_debug_log, Bool);
  MANIFEST_ROOT_RETRIEVE(quit_after_main_window_closed, Bool);
  MANIFEST_ROOT_RETRIEVE(npapi_plugin_directory, String);
  MANIFEST_ROOT_RETRIEVE(disable_ime_composition, Bool);

  Json::Value form = root_["form"];
  std::string form_style_str = form.get("style", "standard").asString();
  std::transform(form_style_str.begin(),
                 form_style_str.end(),
                 form_style_str.begin(),
                 tolower);
  if (form_style_str == "standard") {
    form_style_ = STANDARD;
  } else if (form_style_str == "captionless") {
    form_style_ = CAPTIONLESS;
  } else if (form_style_str == "desktop_widget") {
    form_style_ = DESKTOP_WIDGET;
  }
  MANIFEST_FORM_RETRIEVE(plain, Bool);
  MANIFEST_FORM_RETRIEVE(system_buttons, Bool);
  Json::Value transmission_color_value = form.get("transmission_color", "none");
  std::string transmission_color_string = transmission_color_value.asString();
  int transmission_color_integer = hex::NONE;
  std::transform(transmission_color_string.begin(),
                 transmission_color_string.end(),
                 transmission_color_string.begin(),
                 tolower);
  if (transmission_color_string != "none") {
    if (transmission_color_string == "full") {
      transmission_color_integer = hex::FULL;
    } else if ((transmission_color_string.size() != 4 &&
        transmission_color_string.size() != 7) ||
        (transmission_color_string.at(0) != '#')) {
      transmission_color_string = "none";
    } else {
      transmission_color_string = transmission_color_string.substr(1);
      if (transmission_color_string.size() == 3) {
        std::string tmp = transmission_color_string;
        transmission_color_string.insert(0, tmp, 0, 1);
        transmission_color_string.insert(2, tmp, 1, 1);
        transmission_color_string.insert(4, tmp, 2, 1);
      }
      char* stop_string;
      transmission_color_integer =
          strtol(transmission_color_string.c_str(), &stop_string, 16);
      if (strcmp(stop_string, "")) {
        transmission_color_string = "none";
      }
    }
    if (transmission_color_string == "none") {
      transmission_color_integer = -1;
    }
    form_transmission_color_ = transmission_color_integer;
  }
  MANIFEST_CORRECT_VALUE(form_transmission_color, NONE,
      form_style_ == STANDARD);
  MANIFEST_FORM_RETRIEVE(transparent_browser, Bool);
  form_adjusted_transparent_browser_ = form_transparent_browser_;
  MANIFEST_CORRECT_VALUE(form_adjusted_transparent_browser, false,
      form_transmission_color_ != NONE || !IsAeroGlassEnabled());
  /*MANIFEST_CORRECT_VALUE(form_transparent_browser, false,
      form_transmission_color_ != NONE || !IsAeroGlassEnabled());*/
  MANIFEST_FORM_RETRIEVE(fixed, Bool);
  /*MANIFEST_CORRECT_VALUE(form_plain, true,
      form_style_ == DESKTOP_WIDGET || form_transmission_color_ != NONE);*/
  MANIFEST_CORRECT_VALUE(form_plain, false, form_style_ == STANDARD);
  MANIFEST_CORRECT_VALUE(form_fixed, form_plain_,
      form_style_ != STANDARD && form_transparent_browser_ &&
          (form_transmission_color_ == NONE && IsAeroGlassEnabled()));
  MANIFEST_CORRECT_VALUE(form_opacity, NONE, form_transmission_color_ != NONE);
  MANIFEST_CORRECT_VALUE(form_system_buttons, false,
      !form_transparent_browser_ ||
          (form_transmission_color_ != NONE || !IsAeroGlassEnabled()));

  /*MANIFEST_CORRECT_VALUE(form_borderless, true, form_embed_into_desktop_);
  MANIFEST_CORRECT_VALUE(form_transparent_browser, false,
      !IsAeroGlassEnabled());
  MANIFEST_CORRECT_VALUE(form_no_system_animation, false,
      !IsAeroGlassEnabled());
  MANIFEST_CORRECT_VALUE(form_no_system_shadow, true,
      form_embed_into_desktop_ ||
      form_transmission_color_ != -1 ||
      form_fully_transmission_);
  MANIFEST_CORRECT_VALUE(form_no_system_animation, true,
      form_embed_into_desktop_);
  MANIFEST_CORRECT_VALUE(form_hide_in_taskbar, true, form_embed_into_desktop_);
  MANIFEST_CORRECT_VALUE(form_no_system_animation, false, !form_borderless_);
  MANIFEST_CORRECT_VALUE(form_transparent_browser, false,
      form_transmission_color_ != -1 || form_embed_into_desktop_);
  MANIFEST_CORRECT_VALUE(form_transmission_color, NONE,
      form_embed_into_desktop_);
  MANIFEST_CORRECT_VALUE(form_no_system_shadow, false, !form_borderless_);*/
  MANIFEST_FORM_RETRIEVE(disable_form_apis, Bool);
  Json::Value opacityValue = form.get("opacity", "none");
  if (opacityValue.isInt()) {
    int o = opacityValue.asInt();
    if (o >= 0 && o <= 255) {
      form_opacity_ = o;
      //form_no_system_animation_ = false;
    } else {
      form_opacity_ = NONE;
    }
  } else {
    form_opacity_ = NONE;
  }
  //MANIFEST_CORRECT_VALUE(form_opacity, NONE, form_style_ == DESKTOP_WIDGET);
  //MANIFEST_CORRECT_VALUE(form_no_system_animation, false, form_opacity_ != NONE);
  MANIFEST_FORM_RETRIEVE(hook_system_command, Bool);
  //MANIFEST_FORM_RETRIEVE(hide_in_taskbar, Bool);
  //MANIFEST_CORRECT_VALUE(form_hide_in_taskbar, false, !form_borderless_);
  std::string launchStateStr = form.get("launch_state", "normal").asString();
  std::transform(launchStateStr.begin(),
                 launchStateStr.end(),
                 launchStateStr.begin(),
                 tolower);
  if (launchStateStr == "maximized") {
    form_launch_state_ = MAXIMIZED;
  } else if (launchStateStr == "minimized") {
    form_launch_state_ = MINIMIZED;
  } else if (launchStateStr == "fullscreen") {
    form_launch_state_ = FULLSCREEN;
  } else if (launchStateStr == "normal") {
    form_launch_state_ = NORMAL;
  } else if (launchStateStr == "hidden") {
    form_launch_state_ = HIDDEN;
  } else {
    form_launch_state_ = hex::NORMAL;
  }
  std::string launchWidthStr =
      form.get("launch_width", "system_default").asString();
  std::transform(launchWidthStr.begin(),
                 launchWidthStr.end(),
                 launchWidthStr.begin(),
                 tolower);
  if (launchWidthStr == "system_default") {
    form_launch_width_ = SYSTEMDEFAULT;
  } else {
    MANIFEST_FORM_RETRIEVE(launch_width, Int);
  }
  std::string launchHeightStr =
      form.get("launch_height", "system_default").asString();
  std::transform(launchHeightStr.begin(),
                 launchHeightStr.end(),
                 launchHeightStr.begin(),
                 tolower);
  if (launchHeightStr == "system_default") {
    form_launch_height_ = SYSTEMDEFAULT;
  } else {
    MANIFEST_FORM_RETRIEVE(launch_height, Int);
  }
  std::string form_launch_x =
      form.get("launch_x", form_launch_x_).asString();
  std::transform(form_launch_x.begin(),
                 form_launch_x.end(),
                 form_launch_x.begin(),
                 tolower);
  if (form_launch_x == "system_default") {
    form_launch_x_ = SYSTEMDEFAULT;
  } else if (form_launch_x == "parent_centered") {
    form_launch_x_ = PARENTCENTERED;
  } else if (form_launch_x == "screen_centered") {
    form_launch_x_ = SCREENCENTERED;
  } else if (form.get("launch_left", form_launch_x_).isInt()) {
    MANIFEST_FORM_RETRIEVE(launch_x, Int);
  }
  std::string form_launch_y =
      form.get("launch_y", form_launch_y_).asString();
  std::transform(form_launch_y.begin(),
                 form_launch_y.end(),
                 form_launch_y.begin(),
                 tolower);
  if (form_launch_y == "system_default") {
    form_launch_y_ = SYSTEMDEFAULT;
  } else if (form_launch_y == "parent_centered") {
    form_launch_y_ = PARENTCENTERED;
  } else if (form_launch_y == "screen_centered") {
    form_launch_y_ = SCREENCENTERED;
  } else if (form.get("launch_top", form_launch_y_).isInt()) {
    MANIFEST_FORM_RETRIEVE(launch_y, Int);
  }

  MANIFEST_FORM_RETRIEVE(min_width, Int);
  MANIFEST_FORM_RETRIEVE(min_height, Int);
  MANIFEST_FORM_RETRIEVE(max_width, Int);
  MANIFEST_FORM_RETRIEVE(max_height, Int);
  MANIFEST_FORM_RETRIEVE(border_width, Int);
  MANIFEST_CORRECT_VALUE(form_border_width, 0, form_border_width_ < 0);

  Json::Value browser = root_["browser"];
  MANIFEST_BROWSER_RETRIEVE(no_proxy_server, Bool);
  MANIFEST_BROWSER_RETRIEVE(winhttp_proxy_resolver, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_gpu, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_3d_apis, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_databases, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_experimental_webgl, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_file_system, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_geolocation, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_gpu_process_prelaunch, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_java, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_javascript, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_javascript_i18n_api, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_local_storage, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_logging, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_plugins, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_renderer_accessibility, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_session_storage, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_speech_input, Bool);
  MANIFEST_BROWSER_RETRIEVE(disable_web_sockets, Bool);
  MANIFEST_BROWSER_RETRIEVE(in_process_gpu, Bool);
  MANIFEST_BROWSER_RETRIEVE(in_process_plugins, Bool);
  MANIFEST_BROWSER_RETRIEVE(web_security_disabled, Bool);
  MANIFEST_BROWSER_RETRIEVE(file_access_from_file_urls_allowed, Bool);
  MANIFEST_BROWSER_RETRIEVE(universal_access_from_file_urls_allowed, Bool);

  return true;
}

bool ManifestUtil::UpdateWithJson(std::string json_content) {
  json_content_ = json_content;
  MANIFEST_PARSE();

  return true;
}

}  // namespace hexclient
