// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "libcef/browser/browser_host_impl.h"
#include "libcef/browser/context.h"
#include "libcef/common/cef_messages.h"
#include "content/browser/renderer_host/render_view_host_impl.h"
#include "content/public/browser/web_contents_view.h"
#include "libcef/browser/context.h"
#include "libcef/browser/browser_message_loop.h"
#include "content/public/browser/render_widget_host_view.h"
#if defined(OS_WIN)
#include "hex/src/hex_shared_win.h"
#endif
#include "hex/src/hex_use_node.h"
#include "hex/src/customized.h"

void CefBrowserHostImpl::UvRun(/*int64 frame_id*/) {
  // Only known frame ids are supported.
  //DCHECK(frame_id > CefFrameHostImpl::kMainFrameId);

  Cef_Request_Params params;
  params.name = "uv-run";
  //params.frame_id = frame_id;
  //params.user_initiated = false;
  //params.request_id = -1;
  //params.expect_response = false;

  //if (web_contents_.get())
    Send(new CefMsg_Request(routing_id(), params));
}

void CefBrowserHostImpl::HeXRenderViewCreated(
    content::RenderViewHost* render_view_host) {
  // Set this browser to message loop for executing uv_run. 
  if (hex::CanUseNode()) {
    CefBrowserMessageLoop::current()->SetBrowser(this);
  }
  // FIXME: here may not be the right position for subclassing render
  // widget host view.
#if defined(OS_WIN)
  if (render_view_host->GetView()) {
    if (!CefContext::Get()->hex_settings().hex_module &&
        !renderer_window_subclassed_) {
      renderer_window_handle_ =
          web_contents_->GetRenderWidgetHostView()->GetNativeView();
      renderer_window_thunk_->Init((DWORD_PTR)StaticSubclassRendererWndProc,
          this);
      old_renderer_wndproc_ = (WNDPROC)SetWindowLongPtr(renderer_window_handle_,
          GWLP_WNDPROC, (LONG)renderer_window_thunk_->GetCodeAddress());
      renderer_window_subclassed_ = true;
      SendMessage(browser_window_handle_, WM_TRIGGERNONCLIENTAREA, NULL, NULL);
    }
  }
#endif  // OS_WIN
  HeXSettings hex_settings = CefContext::Get()->hex_settings();
  if (hex_settings.transparent_browser &&
      hex_settings.transmission_color == hex::NONE &&
      hex::IsAeroGlassEnabled() &&
      render_view_host) {
    SkBitmap background;
    background.setConfig(SkBitmap::kARGB_8888_Config, 1, 1);
    background.allocPixels();
    background.eraseARGB(0, 0, 0, 0);

    content::RenderWidgetHostView* view = render_view_host->GetView();
    if (view)
      view->SetBackground(background);
  }
};

void CefBrowserHostImpl::HeXRenderViewDeleted() {
  if (hex::CanUseNode()) {
    CefBrowserMessageLoop::current()->RemoveBrowser();
  }
}
