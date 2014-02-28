// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "client_app.h"
#include "client_renderer.h"

namespace hexclient {

// static
void ClientApp::CreateBrowserDelegates(BrowserDelegateSet& delegates) {
}

// static
void ClientApp::CreateRenderDelegates(RenderDelegateSet& delegates) {
}

// static
void ClientApp::RegisterCustomSchemes(
    CefRefPtr<CefSchemeRegistrar> registrar,
    std::vector<CefString>& cookiable_schemes) {
}

}  // namespace hexclient
