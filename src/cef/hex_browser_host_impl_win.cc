// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "libcef/browser/browser_host_impl.h"
#include <windowsx.h>
#include <dwmapi.h>
#include "libcef/browser/context.h"

#include "base/i18n/case_conversion.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "base/win/registry.h"
#include "base/win/windows_version.h"

#include "hex/include/hex_types_wrappers.h"
#include "hex/src/customized.h"
#include "hex/src/hex_shared.h"
#include "hex/src/hex_shared_win.h"

#pragma comment(lib, "dwmapi.lib")

#define IDT_SHOWSYSTEMMENU 0x4000

// static
LRESULT CALLBACK CefBrowserHostImpl::StaticSubclassBrowserWndProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam) {
  return reinterpret_cast<CefBrowserHostImpl*>(hwnd)->SubclassBrowserWndProc(
      message, wParam, lParam);
}

LRESULT CefBrowserHostImpl::SubclassBrowserWndProc(UINT msg,
                                                   WPARAM wParam,
                                                   LPARAM lParam) {
  HWND hwnd = browser_window_handle_;
  bool top_window = window_info_.parent_window == NULL;
  HeXSettings settings = CefContext::Get()->hex_settings();
  bool can_borderless = settings.form_style != hex::STANDARD && top_window;

  switch (msg) {
    case WM_SYSCOMMAND: {
      hex::SystemCommand command = hex::RESTORE;
      switch (wParam) {
        case SC_RESTORE:
          command = hex::RESTORE;
          break;
        case SC_MOVE:
          command = hex::MOVE;
          break;
        case SC_SIZE:
          command = hex::SIZE;
          break;
        case SC_MINIMIZE:
          command = hex::MINIMIZE;
          break;
        case SC_MAXIMIZE:
          command = hex::MAXIMIZE;
          break;
        case SC_CLOSE:
          command = hex::CLOSE;
          break;
      }
      if (!settings.hook_system_command || is_doing_system_command_directly_) {
        DefWindowProc(hwnd, msg, wParam, lParam);
        EmitNodeEvent(this,
                      L"afterSystemCommand",
                      L"%d",
                      static_cast<int>(command));
        return 0;
      }

      EmitNodeEvent(this,
                    L"beforeSystemCommand",
                    L"%d",
                    static_cast<int>(command));
      is_doing_system_command_directly_ = !can_borderless;
      return 0;
    }
    break;

    case WM_DOSYSCOMMANDDIRECTLY: {
      is_doing_system_command_directly_ = lParam ? true : false;
    }
    break;

    case WM_ACTIVATE: {
      bool isActivation = true;
      switch (wParam) {
        case WA_ACTIVE:
        case WA_CLICKACTIVE:
          isActivation = true;
          break;
        case WA_INACTIVE:
          isActivation = false;
          break;
      }
      EmitNodeEvent(this,
                    L"formActivated",
                    L"%s",
                    isActivation ? _T("true") : _T("false"));
    }
    break;

    case WM_GETPOPUPFORMPOSITION: {
      return MAKELPARAM(popup_position_x_, popup_position_y_);
    }

    case WM_SETPOPUPFORMPOSITION: {
      popup_position_x_ = GET_X_LPARAM(lParam);
      popup_position_y_ = GET_Y_LPARAM(lParam);
    }
    break;

    case WM_GETPOPUPFORMSIZE: {
      return MAKELPARAM(popup_size_width_, popup_size_height_);
    }

    case WM_SETPOPUPFORMSIZE: {
      popup_size_width_ = GET_X_LPARAM(lParam);
      popup_size_height_ = GET_Y_LPARAM(lParam);
    }
    break;

    case WM_GETFORMPOSITION: {
      RECT rect;
      GetWindowRect(hwnd, &rect);
      return MAKELPARAM(rect.left, rect.top);
    }
    break;

    case WM_GETFORMSIZE: {
      RECT rect;
      GetWindowRect(hwnd, &rect);
      return MAKELPARAM(rect.right - rect.left, rect.bottom - rect.top);
    }
    break;

    case WM_GETMINWIDTH: {
      return min_width_;
    }
    break;

    case WM_SETMINWIDTH: {
      min_width_ = (int)lParam;
    }
    break;

    case WM_GETMINHEIGHT: {
      return min_height_;
    }
    break;

    case WM_SETMINHEIGHT: {
      min_height_ = (int)lParam;
    }
    break;

    case WM_GETMAXWIDTH: {
      return max_width_;
    }
    break;

    case WM_SETMAXWIDTH: {
      max_width_ = (int)lParam;
    }
    break;

    case WM_GETMAXHEIGHT: {
      return max_height_;
    }
    break;

    case WM_SETMAXHEIGHT: {
      max_height_ = (int)lParam;
    }
    break;

    case WM_TRIGGERNONCLIENTAREA: {
      if (!can_borderless)
        break;

      SendMessage(renderer_window_handle_, WM_SETASTITLEBARAREAS, 0, 0);
      SendMessage(renderer_window_handle_, WM_LBUTTONDOWN, NULL, MAKELPARAM(0, 0));
      SendMessage(renderer_window_handle_, WM_LBUTTONUP, NULL, MAKELPARAM(0, 0));
      SendMessage(GetAncestor(hwnd, GA_ROOT), WM_NCACTIVATE, WA_ACTIVE, NULL);
    }
    break;

    case WM_REDRAWRENDERER: {
      RedrawWindow(renderer_window_handle_, NULL, NULL, RDW_UPDATENOW | RDW_VALIDATE);
    }
    break;

    case WM_SIZETO: {
      RECT rect;
      GetWindowRect(hwnd, &rect);
      UINT flags = SWP_NOMOVE | SWP_NOACTIVATE;
      if (!wParam) {
        flags |= SWP_NOSENDCHANGING;
      }
      SetWindowPos(hwnd, NULL, 0, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), flags);
      return MAKELPARAM(rect.right - rect.left, rect.bottom - rect.top);
    }
    break;

    case WM_MOVETO: {
      RECT rect;
      GetWindowRect(hwnd, &rect);
      UINT flags = SWP_NOSIZE | SWP_NOACTIVATE;
      if (!wParam) {
        flags |= SWP_NOSENDCHANGING;
      }
      SetWindowPos(hwnd, NULL, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, 0, flags);
      return MAKELPARAM(rect.left, rect.top);
    }
    break;

    case WM_SIZE: {
      if (!can_borderless)
        break;
      // Minimizing resizes the window to 0x0 which causes our layout to go all
      // screwy, so we just ignore it.
      if (wParam != SIZE_MINIMIZED) {
        // resize the web view window to the full size of the browser window
        RECT rc;
        GetClientRect(hwnd, &rc);
        MoveWindow(widget_window_handle_, 0, 0, rc.right, rc.bottom,
            TRUE);
      }
      return 0;
    }
    break;

    case WM_NCHITTEST: {
      /*if (customizedWindow == CUSTOM_NONE) {
        break;
      }*/

      if (top_window) {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);
        int borderWidth = settings.border_width;
        int cornerSize = borderWidth * 1;
        if (x <= rect.left + borderWidth &&
            y >= rect.top + cornerSize &&
            y <= rect.bottom - cornerSize)
          return HTLEFT;
        else if (x >= rect.right - borderWidth &&
                 y >= rect.top + cornerSize &&
                 y <= rect.bottom - cornerSize)
          return HTRIGHT;
        else if (y <= rect.top + borderWidth &&
                 x >= rect.left + cornerSize &&
                 x <= rect.right - cornerSize)
          return HTTOP;
        else if (y >= rect.bottom - borderWidth &&
                 x >= rect.left + cornerSize &&
                 x <= rect.right - cornerSize)
          return HTBOTTOM;
        else if (x <= rect.left + cornerSize && y <= rect.top + cornerSize)
          return HTTOPLEFT;
        else if (x >= rect.right - cornerSize && y <= rect.top + cornerSize)
          return HTTOPRIGHT;
        else if (x <= rect.left + cornerSize && y >= rect.bottom - cornerSize)
          return HTBOTTOMLEFT;
        else if (x >= rect.right - cornerSize && y >= rect.bottom - cornerSize)
          return HTBOTTOMRIGHT;
      } else {
        return HTTRANSPARENT;
      }
    }
    break;

    case WM_NCCALCSIZE: {
      if (!can_borderless) {
        break;
      }

      NCCALCSIZE_PARAMS* sizeParam = (NCCALCSIZE_PARAMS*)lParam;
      if ((BOOL)wParam) {
        MONITORINFO mi = hex::GetMonitorInformation(hwnd);
        sizeParam->rgrc[2] = sizeParam->rgrc[1];
        sizeParam->rgrc[1] = sizeParam->rgrc[0];
        /*if (sizeParam->rgrc[0].left < mi.rcWork.left &&
            sizeParam->rgrc[0].right > mi.rcWork.right &&
            sizeParam->rgrc[0].top < mi.rcWork.top &&
            sizeParam->rgrc[0].bottom > mi.rcWork.bottom) {
          sizeParam->rgrc[0].left = mi.rcWork.left;
          sizeParam->rgrc[0].right = mi.rcWork.right;
          sizeParam->rgrc[0].top = mi.rcWork.top;
          sizeParam->rgrc[0].bottom = mi.rcWork.bottom;
        }*/
      }
      return 0;
    }
    break;

    case WM_GETMINMAXINFO: {
      MINMAXINFO* mmi = (MINMAXINFO*)lParam;
      if (min_width_ > 0)
        mmi->ptMinTrackSize.x = min_width_;
      if (min_height_ > 0)
        mmi->ptMinTrackSize.y = min_height_;
      if (max_width_ > 0)
        mmi->ptMaxTrackSize.x = max_width_;
      if (max_height_ > 0)
        mmi->ptMaxTrackSize.y = max_height_;

      if (!can_borderless) {
        break;
      }

      MONITORINFO mi = hex::GetMonitorInformation(hwnd);
      mmi->ptMaxSize.x = mi.rcWork.right - mi.rcWork.left;
      mmi->ptMaxSize.y = mi.rcWork.bottom - mi.rcWork.top;
      mmi->ptMaxPosition.x = mi.rcWork.left;
      mmi->ptMaxPosition.y = mi.rcWork.top;
      return 0;
    }
    break;

    case WM_COMMAND: {
      switch (wParam) {
        case SC_RESTORE:
        case SC_MOVE:
        case SC_SIZE:
        case SC_MINIMIZE:
        case SC_MAXIMIZE:
        case SC_CLOSE:
          PostMessage(hwnd, WM_SYSCOMMAND, wParam, lParam);
          break;
      }
    }
    break;

    case WM_DWMCOMPOSITIONCHANGED: {
      BOOL enabled;
      if (DwmIsCompositionEnabled(&enabled) == S_OK &&
          settings.transparent_browser) {
        this->GetHost()->SetTransparent(enabled ? true : false);
      }
    }
    break;
  }

  return CallWindowProc(old_browser_wndproc_, browser_window_handle_, msg,
      wParam, lParam);
}

// static
LRESULT CALLBACK CefBrowserHostImpl::StaticSubclassRendererWndProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam) {
  return reinterpret_cast<CefBrowserHostImpl*>(hwnd)->SubclassRendererWndProc(
      message, wParam, lParam);
}

LRESULT CefBrowserHostImpl::SubclassRendererWndProc(UINT msg,
                                                    WPARAM wParam,
                                                    LPARAM lParam) {
  HWND hwnd = renderer_window_handle_;
  bool top_window = window_info_.parent_window == NULL;
  HeXSettings settings = CefContext::Get()->hex_settings();
  bool can_borderless = settings.form_style != hex::STANDARD && top_window;
  static std::map<int, bool> systemMenuEnabledState;

  switch (msg) {
    /*case WM_EXITMENULOOP: {
      SendMessage(GetAncestor(hwnd, GA_ROOT), WM_NCACTIVATE, WA_ACTIVE, NULL);
      //EmitMouseEvent(this, L"exitMenu");
      CefMouseEvent event;
      POINT p;
      GetCursorPos(&p);
      ScreenToClient(hwnd, &p);
      event.x = p.x;
      event.y = p.y;
      SendMouseClickEvent(event, MBT_LEFT, true, 1);
    }
    break;*/

    case WM_TRACKSYSTEMMENU: {
      int x = GET_X_LPARAM(lParam);
      int y = GET_Y_LPARAM(lParam);
      HWND topWindowHandle;
      /*if (hex::DesktopWidget() && GetParent(browser_window_handle_)) {
        topWindowHandle = GetParent(browser_window_handle_);
      } else {*/
        topWindowHandle = top_window
            ? window_info_.window
            : hex::GetMainWindowHandle(GetCurrentProcessId());
      //}
      HMENU systemMenu = GetSystemMenu(topWindowHandle, FALSE);
      TrackPopupMenu(systemMenu,
                            TPM_LEFTALIGN | TPM_RIGHTBUTTON/* | TPM_RETURNCMD*/,
                            x,
                            y,
                            0,
                            (HWND)wParam, NULL);
    }
    break;

    case WM_MOUSEMOVE: {
      if (!settings.system_buttons ||
          !settings.transparent_browser ||
          settings.transmission_color != hex::NONE ||
          !hex::IsAeroGlassEnabled()) {
        break;
      }
      POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
      HWND top = GetAncestor(hwnd, GA_ROOT);
      if (top) {
        ClientToScreen(hwnd, &point);
        DWORD position = MAKELPARAM(point.x, point.y);
        int hit_test = SendMessage(top, WM_NCHITTEST, 0, position);
        PostMessage(top, WM_NCMOUSEMOVE, hit_test, position);
      }
    }
    break;

    case WM_MOUSELEAVE: {
      SendMessage(GetAncestor(hwnd, GA_ROOT), msg, wParam, lParam);
    }
    break;

    case WM_NCHITTEST: {
      /*if (customizedWindow == CUSTOM_NONE) {
        break;
      }*/

      if (settings.system_buttons) {
        LRESULT result;
        BOOL b = DwmDefWindowProc(GetAncestor(hwnd, GA_ROOT),
            msg, wParam, lParam, &result);
        if (b && (result == HTCLOSE || result == HTMINBUTTON ||
            result == HTMAXBUTTON)) {
          return HTTRANSPARENT;
        }
      }

      RECT rect;
      GetWindowRect(hwnd, &rect);
      int border_width = settings.border_width;
      WINDOWPLACEMENT placement;
      GetWindowPlacement(GetAncestor(hwnd, GA_ROOT), &placement);
      if (placement.showCmd == SW_MAXIMIZE) {
        border_width = 0;
      }
      int x = GET_X_LPARAM(lParam);
      int y = GET_Y_LPARAM(lParam);
      POINT point = { x, y };
      POINT screen_position = { x, y };
      //ScreenToClient(hwnd, &point);
      //ScreenToClient(hwnd, &screen_position);
      POINT title_bar_position = { title_bar_x_, title_bar_y_ };
      POINT non_border_position = { non_border_x_, non_border_y_ };
      POINT system_menu_icon_position =
          { system_menu_icon_x_, system_menu_icon_y_ };
      if (title_bar_x_ != -1 && title_bar_y_ != -1)
        ClientToScreen(hwnd, &title_bar_position);
      if (non_border_x_ != -1 && non_border_y_ != -1)
        ClientToScreen(hwnd, &non_border_position);
      if (system_menu_icon_x_ != -1 && system_menu_icon_y_ != -1)
        ClientToScreen(hwnd, &system_menu_icon_position);
      int corner_size = border_width * 2;
      if (-1 != non_border_x_ || -1 != non_border_y_) {
        return HTCLIENT;
      } else 
      //else if (x == non_border_position.x || y == non_border_position.y) {
      if (screen_position.x <= rect.left + border_width &&
        screen_position.y >= rect.top + corner_size &&
        screen_position.y <= rect.bottom - corner_size)
        return HTTRANSPARENT;
      else if (screen_position.x >= rect.right - border_width &&
        screen_position.y >= rect.top + corner_size &&
        screen_position.y <= rect.bottom - corner_size)
        return HTTRANSPARENT;
      else if (screen_position.y <= rect.top + border_width &&
        screen_position.x >= rect.left + corner_size &&
        screen_position.x <= rect.right - corner_size)
        return HTTRANSPARENT;
      else if (screen_position.y >= rect.bottom - border_width &&
        screen_position.x >= rect.left + corner_size &&
        screen_position.x <= rect.right - corner_size)
        return HTTRANSPARENT;
      else if (screen_position.x <= rect.left + corner_size && screen_position.y <= rect.top + corner_size)
        return HTTRANSPARENT;
      else if (screen_position.x >= rect.right - corner_size && screen_position.y <= rect.top + corner_size)
        return HTTRANSPARENT;
      else if (screen_position.x <= rect.left + corner_size && screen_position.y >= rect.bottom - corner_size)
        return HTTRANSPARENT;
      else if (screen_position.x >= rect.right - corner_size && screen_position.y >= rect.bottom - corner_size)
        return HTTRANSPARENT;
      if (point.x == system_menu_icon_position.x && point.y == system_menu_icon_position.y) {
        return HTTRANSPARENT;
      } else if (point.x == title_bar_position.x && point.y == title_bar_position.y)
        return (hex::IsAeroGlassEnabled() && settings.system_buttons) ? HTTRANSPARENT : HTCLIENT;
      else
        return HTCLIENT;
    }
    break;

    case WM_SETASTITLEBARAREAS: {
      title_bar_x_ = wParam;
      title_bar_y_ = lParam;
    }
    break;

    case WM_SETASNONBORDERAREAS: {
      non_border_x_ = wParam;
      non_border_y_ = lParam;
    }
    break;

    case WM_SETASSYSTEMMENUICONAREAS: {
      system_menu_icon_x_ = wParam;
      system_menu_icon_y_ = lParam;
    }
    break;

    case WM_LBUTTONDOWN:
    //case WM_LBUTTONUP:
    case WM_LBUTTONDBLCLK:
    //case WM_RBUTTONUP:
    /*case WM_RBUTTONDOWN:*/ {
      /*if (customizedWindow == CUSTOM_NONE) {
        //break;
      }*/
      int x = GET_X_LPARAM(lParam);
      int y = GET_Y_LPARAM(lParam);
      RECT rect;
      GetClientRect(hwnd, &rect);
      HWND top_window_handle = GetAncestor(hwnd, GA_ROOT);
      //HWND topWindowHandle;
      /*if (hex::DesktopWidget() && GetParent(browser_window_handle_)) {
        topWindowHandle = GetParent(browser_window_handle_);
      } else {*/
        /*topWindowHandle = top_window
            ? window_info_.window
            : hex::GetMainWindowHandle(GetCurrentProcessId());*/
      //}
      /*int hit_test = HTCLIENT;
      if (title_bar_x_ != -1 && title_bar_y_ != -1) {
        hit_test = HTCAPTION;
      }
      if (system_menu_icon_x_ != -1 && system_menu_icon_y_ != -1) {
        hit_test = HTSYSMENU;
      }*/
      //if (non_client_x_ != -1 && non_client_y_ != -1) {
      if ((title_bar_x_ != -1 && title_bar_y_ != -1) ||
          (system_menu_icon_x_ != -1 && system_menu_icon_y_ != -1) ||
          (non_border_x_ != -1 && non_border_y_ != -1)) {
        POINT p = { x, y };
        ClientToScreen(hwnd, &p);
        int hit_test = SendMessage(top_window_handle, WM_NCHITTEST, NULL, MAKELPARAM(p.x, p.y));
        if (hit_test == HTCLIENT)
          break;
        if (msg == WM_LBUTTONDOWN) {
          SendMessage(top_window_handle, WM_DOSYSCOMMANDDIRECTLY, NULL, true);
          /*if (hit_test == HTSYSMENU) {
            clock_t start = clock();
            MSG message;
            while (clock() < start + (double)GetDoubleClickTime()) {
              if (PeekMessage(&message, hwnd, WM_LBUTTONDBLCLK, WM_LBUTTONDBLCLK,
                  PM_NOREMOVE)) {
                SendMessage(top_window_handle, WM_NCLBUTTONDBLCLK, hit_test,
                MAKELPARAM(p.x, p.y));
                return 0;
              } else {
                continue;
              }
            }
          }*/
          SendMessage(top_window_handle, WM_NCLBUTTONDOWN, hit_test,
              MAKELPARAM(p.x, p.y));
          return 0;
        } else if (msg == WM_LBUTTONUP) {
          SendMessage(top_window_handle, WM_DOSYSCOMMANDDIRECTLY, NULL, true);
          SendMessage(top_window_handle, WM_NCLBUTTONUP, hit_test,
              MAKELPARAM(p.x, p.y));
          /*title_bar_x_ = -1;
          title_bar_y_ = -1;
          SendMessage(top_window_handle, WM_SETASTITLEBARAREAS, -1, -1);*/
          return 0;
        } else if (msg == WM_RBUTTONUP) {
          SendMessage(top_window_handle, WM_DOSYSCOMMANDDIRECTLY, NULL, true);
          SendMessage(top_window_handle, WM_NCRBUTTONUP, hit_test,
              MAKELPARAM(p.x, p.y));
          /*title_bar_x_ = -1;
          title_bar_y_ = -1;
          SendMessage(top_window_handle, WM_SETASTITLEBARAREAS, -1, -1);*/
          /*HMENU systemMenu = GetSystemMenu(top_window_handle, FALSE);
          POINT point = (POINT)MAKEPOINTS(lParam);
          ClientToScreen(top_window_handle, &point);
          TrackPopupMenu(systemMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x,
            point.y, 0, top_window_handle, NULL);
          SendMessage(top_window_handle, WM_NCRBUTTONUP, HTCAPTION,
            MAKELPARAM(p.x, p.y));*/
          return 0;
        } else if (msg == WM_RBUTTONDOWN) {
          SendMessage(top_window_handle, WM_DOSYSCOMMANDDIRECTLY, NULL, true);
          SendMessage(top_window_handle, WM_NCRBUTTONDOWN, hit_test,
              MAKELPARAM(p.x, p.y));
          /*title_bar_x_ = -1;
          title_bar_y_ = -1;
          SendMessage(top_window_handle, WM_SETASTITLEBARAREAS, -1, -1);*/
          return 0;
        } else if (msg == WM_LBUTTONDBLCLK) {
          SendMessage(top_window_handle, WM_DOSYSCOMMANDDIRECTLY, NULL, true);
          SendMessage(top_window_handle, WM_NCLBUTTONDBLCLK, hit_test,
              MAKELPARAM(p.x, p.y));
          return 0;
        } /*else if (msg == WM_RBUTTONUP) {
          HMENU systemMenu = GetSystemMenu(topWindowHandle, FALSE);
          POINT point;
          point.x = x;
          point.y = y;
          ClientToScreen(topWindowHandle, &point);
          TrackPopupMenu(systemMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x,
            point.y, 0, topWindowHandle, NULL);
          SendMessage(top_window_handle, WM_NCRBUTTONUP, HTCAPTION,
            MAKELPARAM(p.x, p.y));
          return 0;
        }*/
      }
    }
    break;

    case WM_GETMAXIMIZEDADJUSTEDBORDERWIDTH: {
      if (!hex::IsAeroGlassEnabled() || !settings.system_buttons) {
        return 0;
      }
      NONCLIENTMETRICS ncm;
      ncm.cbSize = sizeof ncm;
      SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);
      return 3 + ncm.iBorderWidth + ncm.iPaddedBorderWidth;
    }
    break;

    case WM_COMMAND: {
      HWND topWindowHandle = top_window
          ? window_info_.window
              : hex::GetMainWindowHandle(GetCurrentProcessId());
      switch (wParam) {
        case SC_RESTORE:
        case SC_MOVE:
        case SC_SIZE:
        case SC_MINIMIZE:
        case SC_MAXIMIZE:
        case SC_CLOSE:
          SendMessage(topWindowHandle, WM_DOSYSCOMMANDDIRECTLY, NULL, true);
          PostMessage(topWindowHandle, WM_SYSCOMMAND, wParam, lParam);
          break;
      }
    }
    break;

    case WM_ENABLESYSTEMMENUITEM: {
      UINT command = (UINT)wParam;
      bool enabled = lParam ? true : false;
      bool prevState = (systemMenuEnabledState.find(command) != systemMenuEnabledState.end()) ? systemMenuEnabledState[command] : true;
      systemMenuEnabledState[command] = enabled;
      return prevState;
    }
    break;


    case WM_INITMENUPOPUP: {
      HWND top = top_window
          ? window_info_.window
              : hex::GetMainWindowHandle(GetCurrentProcessId());
      HMENU systemMenu = GetSystemMenu(top, FALSE);
      if ((HMENU)wParam == systemMenu) {
        int count = GetMenuItemCount(systemMenu);
        for (int i = 0; i < count; ++i) {
          UINT uID = GetMenuItemID(systemMenu, i);
          bool userState = systemMenuEnabledState.find(uID) != systemMenuEnabledState.end() ? systemMenuEnabledState[uID] : true;
          bool shouldBeDefault = false;
          bool shouldBeDisabled = false;
          switch (uID) {
            case SC_RESTORE:
              if (IsIconic(top)) {
                SetMenuDefaultItem(systemMenu, i, MF_BYPOSITION);
              }
              if (!IsZoomed(top) && !IsIconic(top)) {
                shouldBeDisabled = true;
              }
              
              EnableMenuItem(systemMenu, i,
                  MF_BYPOSITION | (!shouldBeDisabled ? MF_ENABLED : MF_GRAYED));
              break;

            case SC_MAXIMIZE:
              if (IsZoomed(top)) {
                shouldBeDisabled = true;
              }
              EnableMenuItem(systemMenu, i,
                MF_BYPOSITION | (!shouldBeDisabled && userState ? MF_ENABLED : MF_GRAYED));
              break;
            case SC_MINIMIZE:
              if (IsIconic(top)) {
                shouldBeDisabled = true;
              }
              EnableMenuItem(systemMenu, i,
                MF_BYPOSITION | (!shouldBeDisabled && userState ? MF_ENABLED : MF_GRAYED));
              break;
            case SC_MOVE:
            case SC_SIZE:
              if (IsIconic(top) || IsZoomed(top)) {
                shouldBeDisabled = true;
              }
              EnableMenuItem(systemMenu, i,
                MF_BYPOSITION | (!shouldBeDisabled && userState ? MF_ENABLED : MF_GRAYED));
              break;
            case SC_CLOSE:
              if (!IsIconic(top)) {
                SetMenuDefaultItem(systemMenu, i, MF_BYPOSITION);
              }
              EnableMenuItem(systemMenu, i, MF_BYPOSITION | (userState ? MF_ENABLED : MF_GRAYED));
              break;
            default:
              EnableMenuItem(systemMenu, i, MF_BYPOSITION | (userState ? MF_ENABLED : MF_GRAYED));
          }
        }
      }
    }
    break;

  }

  return CallWindowProc(old_renderer_wndproc_, renderer_window_handle_,
      msg, wParam, lParam);
}

// static
LRESULT CALLBACK CefBrowserHostImpl::StaticSubclassWidgetWndProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam) {
  return reinterpret_cast<CefBrowserHostImpl*>(hwnd)->SubclassWidgetWndProc(
      message, wParam, lParam);
}

LRESULT CefBrowserHostImpl::SubclassWidgetWndProc(UINT msg,
                                                  WPARAM wParam,
                                                  LPARAM lParam) {
  HWND hwnd = widget_window_handle_;
  //bool topWindow = window_info_.parent_window == NULL;
  //CefSettings settings = _Context->settings();
  //bool customizedWindow = settings.customized_window;
  //bool canCustomize = customizedWindow && topWindow;

  switch (msg) {
    case WM_SHOWWINDOW: {
    }
    break;

    case WM_NCHITTEST: {
      return HTTRANSPARENT;
    }
    break;
    /*case WM_PARENTSIZE:
    case WM_SIZE: {
      if (!settings.round_rect || !topWindow)
        break;
      if (wParam == SIZE_MAXIMIZED) {
        hexclient::UnroundCornerWindow(hwnd);
      } else {
        hexclient::RoundCornerWindow(hwnd);
      }
      SendMessage(renderer_window_handle_, msg, wParam, lParam);
      return 0;
    }
    break;*/
  }

  return CallWindowProc(old_widget_wndproc_, widget_window_handle_,
      msg, wParam, lParam);
}
