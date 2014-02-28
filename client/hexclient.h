// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#ifndef HEX_CLIENT_HEXCLIENT_H_
#define HEX_CLIENT_HEXCLIENT_H_
#pragma once

#include <string>
#include "include/cef_base.h"
#include "../hex/include/hex_types.h"
#include "client_app.h"
#include "manifest_util.h"
#include "client_handler.h"

class CefApp;
class CefBrowser;
class CefCommandLine;

namespace hexclient {

// Returns the main browser window instance.
CefRefPtr<CefBrowser> AppGetBrowser();

// Returns the main application window handle.
CefWindowHandle AppGetMainHwnd();

// Quit the application message loop.
void AppQuitMessageLoop();

// Returns the application settings based on manifest.json file.
void AppGetSettingsFromManifest(CefSettings& settings,
                                HeXSettings& hex_settings,
                                CefRefPtr<ClientApp> app,
                                ManifestUtil& manifest);

// Returns the application browser settings based on manifest.json file.
void AppGetBrowserSettingsFromManifest(CefBrowserSettings& settings,
                                       ManifestUtil& manifest);

}  // namespace hexclient

#endif  // HEX_CLIENT_HEXCLIENT_H_
