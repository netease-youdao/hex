// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#ifndef HEX_CLIENT_CLIENT_RENDERER_H_
#define HEX_CLIENT_CLIENT_RENDERER_H_
#pragma once

#include "include/cef_base.h"
#include "client_app.h"

namespace hexclient {

// Message sent when the focused node changes.
extern const char kFocusedNodeChangedMessage[];

// Create the render delegate.
void CreateRenderDelegates(ClientApp::RenderDelegateSet& delegates);

}  // namespace hexclient

#endif  // HEX_CLIENT_CLIENT_RENDERER_H_
