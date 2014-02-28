// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "libcef/browser/browser_message_loop.h"
#include "libcef/browser/context.h"
#include "libcef/common/content_client.h"
#include "third_party/node/src/node.h"

#if defined(OS_WIN)
bool CefBrowserMessageLoop::PreTranslateMessage(const MSG& msg) {
  CefRefPtr<CefApp> app = CefContentClient::Get()->application();
  if (app.get()) {
    CefRefPtr<CefBrowserProcessHandler> handler =
      app->GetBrowserProcessHandler();
    if (handler.get()) {
      return handler->OnPreTranslateMessage(const_cast<CefEventHandle>(&msg));
    }
  }

  return false;
}
#endif  // OS_WIN

void CefBrowserMessageLoop::UvRun() {
  if (browser_ &&
      /*browser_->GetMainFrame().get() &&
      browser_->GetMainFrame()->GetIdentifier() >
          CefFrameHostImpl::kMainFrameId &&*/
      !CefContext::Get()->hex_settings().disable_async_node_apis &&
      current() &&
      current()->thread_name() == "CrBrowserMain")
    browser_->UvRun(/*browser_->GetMainFrame()->GetIdentifier()*/);
}

void CefBrowserMessageLoop::SetBrowser(CefBrowserHostImpl* browser) { 
  if (!browser_) {
    browser_ = browser; 
  }
}

void CefBrowserMessageLoop::RemoveBrowser() {
  if (browser_) {
    browser_ = NULL;
  }
}
