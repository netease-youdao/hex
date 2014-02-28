// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "hexclient.h"
#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <direct.h>
#include <sstream>
#include <string>
#include <atlconv.h>
#include <Shlwapi.h>
#include <tchar.h>
#include <windowsx.h>
#include <process.h>
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "include/hex.h"
#include "client_handler.h"
#include "client_switches.h"
#include "resource.h"
#include "string_util.h"
#include "customized.h"
#include "hex_shared_win.h"
#include <dwmapi.h>


#define MAX_LOADSTRING 100
#define MAX_URL_LENGTH  255
#define BUTTON_WIDTH 72
#define URLBAR_HEIGHT  24

namespace hexclient {

// Global Variables:
HINSTANCE hInst;   // current instance
TCHAR szTitle[MAX_LOADSTRING];  // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];  // the main window class name
TCHAR szOSRWindowClass[MAX_LOADSTRING];  // the OSR window class name
bool customizedMainWindow = false;

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

// Used for processing messages on the main application thread while running
// in multi-threaded message loop mode.
HWND hMessageWnd = NULL;
HWND CreateMessageWindow(HINSTANCE hInstance);
LRESULT CALLBACK MessageWndProc(HWND, UINT, WPARAM, LPARAM);

// The global ClientHandler reference.
extern CefRefPtr<ClientHandler> g_handler = NULL;

#if defined(OS_WIN)
// Add Common Controls to the application manifest because it's required to
// support the default tooltip implementation.
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")  // NOLINT(whitespace/line_length)
#endif

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this
//    function so that the application will get 'well formed' small icons
//    associated with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEX wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
  ManifestUtil& manifest = ManifestUtil::Instance();
  if (!IsAeroGlassEnabled() && !manifest.FormPlain()) {
    wcex.style |= CS_DROPSHADOW;
  }

  wcex.lpfnWndProc   =  WndProc;
  wcex.cbClsExtra    = 0;
  wcex.cbWndExtra    = 0;
  wcex.hInstance     = hInstance;
  wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HEX));
  wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = NULL;
  wcex.lpszMenuName  = NULL;
  wcex.lpszClassName = hex::GetMainWindowClassName(_getpid());
  wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_HEXSMALL));

  return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  HWND hWnd;
  
  hInst = hInstance;  // Store instance handle in our global variable

  int x = CW_USEDEFAULT;
  int y = 0;
  USES_CONVERSION;
  ManifestUtil& manifest = ManifestUtil::Instance();

  if (manifest.FormLaunchX() != hex::SYSTEMDEFAULT &&
      manifest.FormLaunchY() != hex::SYSTEMDEFAULT) {
    if (manifest.FormLaunchWidth() >= 0) {
      if (manifest.FormLaunchX() < hex::SYSTEMDEFAULT) {
        MONITORINFO mi = hex::GetMonitorInformation(HWND_DESKTOP);
        x = (mi.rcWork.right - mi.rcWork.left -
            manifest.FormLaunchWidth()) / 2;
      } else if (manifest.FormLaunchX() > hex::SYSTEMDEFAULT) {
        x = manifest.FormLaunchX();
      }
    }
    if (manifest.FormLaunchHeight() >= 0) {
      if (manifest.FormLaunchY() < hex::SYSTEMDEFAULT) {
        MONITORINFO mi = hex::GetMonitorInformation(HWND_DESKTOP);
        y = (mi.rcWork.bottom - mi.rcWork.top -
            manifest.FormLaunchHeight()) / 2;
      } else if (manifest.FormLaunchY() > hex::SYSTEMDEFAULT) {
        y = manifest.FormLaunchY();
      }
    }
  }

  DWORD style = WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CAPTION | WS_CLIPCHILDREN |
      WS_SYSMENU | WS_POPUP;
  if (!manifest.FormFixed()) {
    style |= WS_SIZEBOX;
  }

  hWnd = CreateWindowEx(NULL,
                        hex::GetMainWindowClassName(_getpid()),
                        szTitle,
                        style,
                        x,
                        y,
                        manifest.FormLaunchWidth() < 0
                            ? CW_USEDEFAULT
                            : manifest.FormLaunchWidth(),
                        manifest.FormLaunchHeight() < 0
                            ? 0
                            : manifest.FormLaunchHeight(),
                        NULL,
                        NULL,
                        hInstance,
                        NULL);
  if (customizedMainWindow && (manifest.FormAdjustedTransparentBrowser() ||
      manifest.FormTransmissionColor() > hex::NONE) &&
      !manifest.FormSystemButtons()) {
    SetWindowLong(hWnd, GWL_STYLE,
        GetWindowLong(hWnd, GWL_STYLE) & ~(WS_BORDER));
  }
  if (manifest.FormOpacity() != hex::NONE ||
      (manifest.FormTransmissionColor() != hex::NONE &&
      manifest.FormTransmissionColor() != hex::FULL)) {
    DWORD exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
    exStyle |= WS_EX_LAYERED;
    SetWindowLong(hWnd, GWL_EXSTYLE, exStyle);
    if (manifest.FormOpacity() != hex::NONE) {
      SetLayeredWindowAttributes(hWnd, NULL, manifest.FormOpacity(), LWA_ALPHA);
    }
    if (manifest.FormTransmissionColor() != hex::NONE) {
      SetLayeredWindowAttributes(hWnd, manifest.FormTransmissionColor(), 0,
          LWA_COLORKEY);
    }
  }
  if (manifest.FormTransmissionColor() == hex::FULL) {
    DWORD exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
    exStyle |= WS_EX_TRANSPARENT | WS_EX_LAYERED;
    SetWindowLong(hWnd, GWL_EXSTYLE, exStyle);
    SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
  }
  if (!hWnd)
    return FALSE;

  if (manifest.FormStyle() == hex::DESKTOP_WIDGET) {
    HWND desktop = FindWindow(L"Progman", L"Program Manager");
    HWND shell_dll = NULL;
    static HWND old_parent = NULL;
    if (desktop) {
      shell_dll = FindWindowEx(desktop, NULL, L"SHELLDLL_DefView", NULL);
    }
    if (!shell_dll) {
      HWND worker_w = FindWindow(L"WorkerW", NULL);
      while (!shell_dll && worker_w) {
        shell_dll = FindWindowEx(worker_w, NULL, L"SHELLDLL_DefView", NULL);
        worker_w = FindWindowEx(NULL, worker_w, L"WorkerW", NULL);
      }
    }
    if (shell_dll && old_parent != shell_dll) {
      SetWindowLong(hWnd, GWL_HWNDPARENT, (LONG)shell_dll);
      old_parent = shell_dll;
    }
  }

  switch(manifest.FormLaunchState()) {
    case hex::MAXIMIZED:
    case hex::MINIMIZED:
    case hex::FULLSCREEN:
      hex::SetState(hWnd, manifest.FormLaunchState(), true);
      UpdateWindow(hWnd);
      break;
    case hex::NORMAL:
      ShowWindow(hWnd, nCmdShow);
      UpdateWindow(hWnd);
      break;
    case hex::HIDDEN:
      break;
  }
  if ((manifest.FormAdjustedTransparentBrowser() && !customizedMainWindow) ||
      (customizedMainWindow && IsAeroGlassEnabled() &&
      manifest.FormTransmissionColor() <= hex::NONE) ||
      (manifest.FormTransmissionColor() > hex::NONE && !manifest.FormPlain())) {
    hex::SetAeroGlass(hWnd, manifest.FormLaunchState() == MAXIMIZED);
  }
  if (manifest.FormStyle() == hex::DESKTOP_WIDGET) {
    SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 0, 0,
       SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
  }

  RECT rect;
  DwmGetWindowAttribute(hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &rect, sizeof rect);
  NONCLIENTMETRICS ncm;
  ncm.cbSize = sizeof (ncm);
  SystemParametersInfo (SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);

  return TRUE;
}

// Change the zoom factor on the UI thread.
static void ModifyZoom(CefRefPtr<CefBrowser> browser, double delta) {
  if (CefCurrentlyOn(TID_UI)) {
    browser->GetHost()->SetZoomLevel(
        browser->GetHost()->GetZoomLevel() + delta);
  } else {
    CefPostTask(TID_UI, NewCefRunnableFunction(ModifyZoom, browser, delta));
  }
}

HWND CreateMessageWindow(HINSTANCE hInstance) {
  static const wchar_t kWndClass[] = L"ClientMessageWindow";

  WNDCLASSEX wc = {0};
  wc.cbSize = sizeof(wc);
  wc.lpfnWndProc = MessageWndProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = kWndClass;
  RegisterClassEx(&wc);

  return CreateWindow(kWndClass, 0, 0, 0, 0, 0, 0, HWND_MESSAGE, 0,
                      hInstance, 0);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  ManifestUtil& manifest = ManifestUtil::Instance();
  static int title_bar_x_ = hex::SYSTEMDEFAULT;
  static int title_bar_y_ = hex::SYSTEMDEFAULT;
  static int non_border_x_ = hex::SYSTEMDEFAULT;
  static int non_border_y_ = hex::SYSTEMDEFAULT;
  static int system_menu_icon_x_ = hex::SYSTEMDEFAULT;
  static int system_menu_icon_y_ = hex::SYSTEMDEFAULT;
  static bool modelDialog = false;
  static int minWidth = manifest.FormMinWidth();
  static int minHeight = manifest.FormMinHeight();
  static int maxWidth = manifest.FormMaxWidth();
  static int maxHeight = manifest.FormMaxHeight();
  static bool isDoingSystemCommandDirectly = !customizedMainWindow;

  PAINTSTRUCT ps;
  HDC hdc;

  switch (message) {
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
      if (!manifest.FormHookSystemCommand() || isDoingSystemCommandDirectly) {
        DefWindowProc(hWnd, message, wParam, lParam);
        EmitNodeEvent(g_handler->GetBrowser(),
                      L"afterSystemCommand",
                      L"%d",
                      static_cast<int>(command));
        return 0;
      }

      EmitNodeEvent(g_handler->GetBrowser(),
                    L"beforeSystemCommand",
                    L"%d",
                    static_cast<int>(command));
      isDoingSystemCommandDirectly = !customizedMainWindow;
      return 0;
    }
    break;

    case WM_DOSYSCOMMANDDIRECTLY: {
      isDoingSystemCommandDirectly = lParam ? true : false;
    }
    break;

    case WM_WINDOWPOSCHANGING: {
      WINDOWPOS* pos = (WINDOWPOS*)lParam;
      EmitNodeEvent(g_handler->GetBrowser(),
                    L"formMoving",
                    L"%s",
                    L"''");
    }
    break;

    case WM_NCPAINT: {
      if (customizedMainWindow && !IsAeroGlassEnabled()) {
        return 0;
      }
    }
    break;

    case WM_EXITMENULOOP: {
      SendMessage(hWnd, WM_NCACTIVATE, WA_ACTIVE, NULL);
      EmitNodeEvent(g_handler->GetBrowser(),
                    L"exitMenu",
                    L"%s",
                    L"''");
    }
    break;

    case WM_NCACTIVATE: {
      bool isActivation = true;
      LRESULT res = 0;
      if (!IsWindowsXP()) {
        if (customizedMainWindow) {
          if ((manifest.FormAdjustedTransparentBrowser() &&
              !manifest.FormSystemButtons()) ||
              manifest.FormTransmissionColor() > hex::NONE ||
              !IsAeroGlassEnabled()) {
            SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
          }
          if ((!manifest.FormAdjustedTransparentBrowser() ||
              manifest.FormSystemButtons()) &&
              manifest.FormTransmissionColor() == hex::NONE &&
              IsAeroGlassEnabled()) {
            res = DefWindowProc(hWnd, message, wParam, lParam);
          }
          if ((manifest.FormAdjustedTransparentBrowser() &&
              !manifest.FormSystemButtons()) ||
              manifest.FormTransmissionColor() > hex::NONE ||
              !IsAeroGlassEnabled()) {
            SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
            RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
          }
        } else {
          res = DefWindowProc(hWnd, message, wParam, lParam);
        }
      }
      switch (wParam) {
        case WA_ACTIVE:
        case WA_CLICKACTIVE:
          isActivation = true;
          break;
        case WA_INACTIVE:
          isActivation = false;
          break;
      }
      EmitNodeEvent(g_handler->GetBrowser(),
                    L"formActivated",
                    L"%s",
                    isActivation ? L"true" : L"false");
      return (IsWindowsXP()) ? TRUE : res;
    }

    case WM_NCCALCSIZE: {
      if (!customizedMainWindow) {
        break;
      }
      NCCALCSIZE_PARAMS* size_param = (NCCALCSIZE_PARAMS*)lParam;
      if ((BOOL)wParam && lParam != NULL) {
        MONITORINFO mi = hex::GetMonitorInformation(hWnd);
        size_param->rgrc[2] = size_param->rgrc[1];
        size_param->rgrc[1] = size_param->rgrc[0];
        if (!IsAeroGlassEnabled() || !manifest.FormSystemButtons()) {
          if (size_param->rgrc[0].left < mi.rcWork.left &&
            size_param->rgrc[0].right > mi.rcWork.right &&
            size_param->rgrc[0].top < mi.rcWork.top &&
            size_param->rgrc[0].bottom > mi.rcWork.bottom) {
              size_param->rgrc[0].left = mi.rcWork.left;
              size_param->rgrc[0].right = mi.rcWork.right;
              size_param->rgrc[0].top = mi.rcWork.top;
              size_param->rgrc[0].bottom = mi.rcWork.bottom;
          }
        }
      }
      return 0;
    }
    break;

    case WM_GETMINMAXINFO: {
      MINMAXINFO* mmi = (MINMAXINFO*)lParam;
      if (minWidth > 0)
        mmi->ptMinTrackSize.x = minWidth;
      if (minHeight > 0)
        mmi->ptMinTrackSize.y = minHeight;
      if (maxWidth > 0)
        mmi->ptMaxTrackSize.x = maxWidth;
      if (maxHeight > 0)
        mmi->ptMaxTrackSize.y = maxHeight;

      if (!customizedMainWindow) {
        break;
      }

      MONITORINFO mi = hex::GetMonitorInformation(hWnd);
      if (customizedMainWindow && (manifest.FormAdjustedTransparentBrowser() ||
          manifest.FormTransmissionColor() > hex::NONE)) {
        mmi->ptMaxSize.x = mi.rcWork.right - mi.rcWork.left;
        mmi->ptMaxSize.y = mi.rcWork.bottom - mi.rcWork.top;
        mmi->ptMaxPosition.x = 0;
        mmi->ptMaxPosition.y = 0;
      }
      return 0;
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

    case WM_DWMCOMPOSITIONCHANGED: {
      BOOL enabled;
      if (DwmIsCompositionEnabled(&enabled) == S_OK &&
          manifest.FormTransparentBrowser()) {
        g_handler->GetBrowser()->GetHost()->SetTransparent(
            enabled ? true : false);
        DWORD class_long = GetClassLong(hWnd, GCL_STYLE);
        if (enabled) {
          SetClassLong(hWnd, GCL_STYLE, class_long & ~(CS_DROPSHADOW));
          SetAeroGlass(hWnd, true);
        } else {
          SetClassLong(hWnd, GCL_STYLE, class_long | CS_DROPSHADOW);
        }
        ShowWindow(hWnd, SW_HIDE);
        ShowWindow(hWnd, SW_SHOW);
      }
      EmitNodeEvent(g_handler->GetBrowser(),
                    L"formAeroChanged",
                    L"%s",
                    enabled ? L"true" : L"false");
    }
    break;

    case WM_MOUSELEAVE: {
      EmitNodeEvent(g_handler->GetBrowser(),
                    L"formMouseLeave",
                    L"%s",
                    L"''");
    }
    break;

    case WM_NCHITTEST: {
      if (!customizedMainWindow ||
          manifest.FormLaunchState() == hex::FULLSCREEN) {
        break;
      }

      if (manifest.FormAdjustedTransparentBrowser() && manifest.FormSystemButtons()) {
        LRESULT lResult;
        BOOL b = DwmDefWindowProc(hWnd, message, wParam, lParam, &lResult);
        if (lResult == HTCLOSE || lResult == HTMINBUTTON || lResult == HTMAXBUTTON) {
          return lResult;
        }
      }

      RECT rect;
      GetWindowRect(hWnd, &rect);
      int x = GET_X_LPARAM(lParam);
      int y = GET_Y_LPARAM(lParam);
      POINT p = { x, y };
      POINT screen_position = { x, y };
      int borderWidth = manifest.FormBorderWidth();
      int cornerSize = borderWidth * 2;
      if (non_border_x_ != -1 || non_border_y_ != -1) {
        return HTCLIENT;
      } else if (screen_position.x <= rect.left + borderWidth &&
        screen_position.y >= rect.top + cornerSize &&
        screen_position.y <= rect.bottom - cornerSize)
        return HTLEFT;
      else if (screen_position.x >= rect.right - borderWidth &&
        screen_position.y >= rect.top + cornerSize &&
        screen_position.y <= rect.bottom - cornerSize)
        return HTRIGHT;
      else if (screen_position.y <= rect.top + borderWidth &&
        screen_position.x >= rect.left + cornerSize &&
        screen_position.x <= rect.right - cornerSize)
        return HTTOP;
      else if (screen_position.y >= rect.bottom - borderWidth &&
        screen_position.x >= rect.left + cornerSize &&
        screen_position.x <= rect.right - cornerSize)
        return HTBOTTOM;
      else if (screen_position.x <= rect.left + cornerSize && screen_position.y <= rect.top + cornerSize)
        return HTTOPLEFT;
      else if (screen_position.x >= rect.right - cornerSize && screen_position.y <= rect.top + cornerSize)
        return HTTOPRIGHT;
      else if (screen_position.x <= rect.left + cornerSize && screen_position.y >= rect.bottom - cornerSize)
        return HTBOTTOMLEFT;
      else if (screen_position.x >= rect.right - cornerSize && screen_position.y >= rect.bottom - cornerSize)
        return HTBOTTOMRIGHT;
      else if (system_menu_icon_x_ != -1 && system_menu_icon_y_ != -1) {
        return HTSYSMENU;
      } else if (title_bar_x_ != -1 && title_bar_y_ != -1) {
        return HTCAPTION;
      }
      else 
        return HTCLIENT;
    }
    break;


    case WM_SIZETO: {
      RECT rect;
      GetWindowRect(hWnd, &rect);
      UINT flags = SWP_NOMOVE | SWP_NOACTIVATE;
      if (!wParam) {
        flags |= SWP_NOSENDCHANGING;
      }
      SetWindowPos(hWnd, NULL, 0, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), flags);
      return MAKELPARAM(rect.right - rect.left, rect.bottom - rect.top);
    }
    break;

    case WM_MOVETO: {
      RECT rect;
      GetWindowRect(hWnd, &rect);
      UINT flags = SWP_NOSIZE | SWP_NOACTIVATE;
      if (!wParam) {
        flags |= SWP_NOSENDCHANGING;
      }
      SetWindowPos(hWnd, NULL, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, 0, flags);
      return MAKELPARAM(rect.left, rect.top);
    }
    break;

    case WM_SETTRANSPARENCY: {
      return SetLayeredWindowAttributes(hWnd, NULL, (BYTE)wParam, LWA_ALPHA);
    }
    break;

    case WM_GETTRANSPARENCY: {
      BYTE alpha;
      return GetLayeredWindowAttributes(hWnd, NULL, &alpha, NULL) ? alpha : -1;
    }
    break;

    case WM_GETFORMPOSITION: {
      RECT rect;
      GetWindowRect(hWnd, &rect);
      return MAKELPARAM(rect.left, rect.top);
    }
    break;

    case WM_GETFORMSIZE: {
      RECT rect;
      GetWindowRect(hWnd, &rect);
      return MAKELPARAM(rect.right - rect.left, rect.bottom - rect.top);
    }
    break;

    case WM_CREATE: {
      // Create the single static handler class instance
      g_handler = new ClientHandler();
      g_handler->SetMainHwnd(hWnd);

      // Create the child windows used for navigation
      RECT rect;
      int x = 0;

      GetClientRect(hWnd, &rect);

      CefWindowInfo info;
      CefBrowserSettings settings;
      AppGetBrowserSettingsFromManifest(settings, manifest);

      info.SetAsChild(hWnd, rect);

      // Creat the new child browser window
      CefBrowserHost::CreateBrowser(info,
                                    g_handler.get(),
                                    ProcessAppDir(manifest.FirstPage()),
                                    settings,
                                    NULL);

      return 0;
    }

    case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);
      EndPaint(hWnd, &ps);
      return 0;

    case WM_SETFOCUS:
      if (g_handler.get() && g_handler->GetBrowser()) {
        // Pass focus to the browser window
        CefWindowHandle hwnd =
            g_handler->GetBrowser()->GetHost()->GetWindowHandle();
        if (hwnd)
          PostMessage(hwnd, WM_SETFOCUS, wParam, NULL);
      }
      return 0;

    case WM_SIZE: {
      // Minimizing resizes the window to 0x0 which causes our layout to go all
      // screwy, so we just ignore it.
      if (wParam != SIZE_MINIMIZED && g_handler.get() &&
          g_handler->GetBrowser()) {
        CefWindowHandle hwnd =
            g_handler->GetBrowser()->GetHost()->GetWindowHandle();
        if (hwnd) {
          // Resize the browser window and address bar to match the new frame
          // window size
          RECT rect;
          GetClientRect(hWnd, &rect);

          SetWindowPos(hwnd, NULL, rect.left, rect.top,
              rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
        }
        if (customizedMainWindow && manifest.FormAdjustedTransparentBrowser()) {
          hex::SetAeroGlass(hWnd, wParam == SIZE_MAXIMIZED);
        }
      }
      SystemCommand command;
      switch (wParam) {
        case SIZE_RESTORED:
          command = hex::RESTORE;
          break;
        case SIZE_MINIMIZED:
          command = hex::MINIMIZE;
          break;
        case SIZE_MAXIMIZED:
          command = hex::MAXIMIZE;
          break;
        default:
          command = hex::SIZE;
          break;
      }
      EmitNodeEvent(g_handler->GetBrowser(),
                    L"formResized",
                    L"%d",
                    static_cast<int>(command));
    }
    break;

    case WM_ERASEBKGND:
      if (g_handler.get() && g_handler->GetBrowser()) {
        CefWindowHandle hwnd =
            g_handler->GetBrowser()->GetHost()->GetWindowHandle();
        if (hwnd) {
          // Dont erase the background if the browser window has been loaded
          // (this avoids flashing)
          return 0;
        }
      }
      break;

    case WM_CLOSE:
      if (g_handler.get() && !g_handler->IsClosing()) {
        CefRefPtr<CefBrowser> browser = g_handler->GetBrowser();
        if (browser.get()) {
          // Notify the browser window that we would like to close it. This
          // will result in a call to ClientHandler::DoClose() if the
          // JavaScript 'onbeforeunload' event handler allows it.
          if (!manifest.QuitAfterMainWindowClosed()) {
            browser->GetHost()->CloseBrowser(false);
            // Cancel the close.
            return 0;
          } else {
            browser->GetHost()->ParentWindowWillClose();
          }
        }
      }
      // Allow the close.
      break;

    case WM_DESTROY:
      if (manifest.QuitAfterMainWindowClosed())
        PostQuitMessage(0);
      // Quitting CEF is handled in ClientHandler::OnBeforeClose().
      return 0;

    case WM_DESTROYMAIN:
      return DestroyWindow(hWnd);
      break;

  }

  return DefWindowProc(hWnd, message, wParam, lParam);
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);
  switch (message) {
  case WM_INITDIALOG:
    return (INT_PTR)TRUE;

  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
      EndDialog(hDlg, LOWORD(wParam));
      return (INT_PTR)TRUE;
    }
    break;
  }
  return (INT_PTR)FALSE;
}

LRESULT CALLBACK MessageWndProc(HWND hWnd, UINT message, WPARAM wParam,
                                LPARAM lParam) {
  switch (message) {
    case WM_COMMAND: {
      int wmId = LOWORD(wParam);
      switch (wmId) {
        case ID_QUIT:
          PostQuitMessage(0);
          return 0;
      }
    }
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}


// Global functions

void AppQuitMessageLoop() {
  CefQuitMessageLoop();
}

}  // namespace hexclient

using namespace hexclient;

// Program entry point function.
int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPTSTR    lpCmdLine,
                      int       nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);
  USES_CONVERSION;

  CefMainArgs main_args(hInstance);
  CefRefPtr<ClientApp> app(new ClientApp);

  // Execute the secondary process, if any.
  int exit_code = CefExecuteProcess(main_args, app.get());
  if (exit_code >= 0)
    return exit_code;

  InitUtil();

  ManifestUtil& manifest = ManifestUtil::Instance();
  if (!manifest.ReadManifest()) {
    MessageBox(NULL, _T("Read manifest.json error!"), NULL,
        MB_OK | MB_ICONERROR);
    return 0;
  }
  if (!manifest.Parse()) {
    WCHAR message[512] = _T("Parse manifest.json error!\n");
    wcscat_s(message, _countof(message), A2W(manifest.ErrorMessages().c_str()));
    MessageBox(NULL, message, NULL, MB_OK | MB_ICONERROR);
    return 0;
  }

  CefSettings settings;
  HeXSettings hex_settings;

  // Populate the settings based on command line arguments.
  AppGetSettingsFromManifest(settings, hex_settings, app, manifest);
  customizedMainWindow = manifest.FormStyle() != hex::STANDARD;

  // Initialize CEF.
  CefInitialize(main_args, settings, app.get());
  HeXSettingsInitialize(hex_settings);

  if (manifest.NpapiPluginDirectory().empty()) {
    CefAddWebPluginDirectory(GetWorkingDirectory());
  } else {
    CefAddWebPluginDirectory(A2W(manifest.NpapiPluginDirectory().c_str()));
  }
  CefRefreshWebPlugins();

  HACCEL hAccelTable;

  // Initialize global strings
  LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadString(hInstance, IDC_CEFCLIENT, szWindowClass, MAX_LOADSTRING);
  LoadString(hInstance, IDS_OSR_WIDGET_CLASS, szOSRWindowClass, MAX_LOADSTRING);
  hexclient::MyRegisterClass(hInstance);

  // Perform application initialization
  if (!InitInstance(hInstance, nCmdShow))
    return FALSE;

  hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CEFCLIENT));

  int result = 0;

  if (!settings.multi_threaded_message_loop) {
    // Run the CEF message loop. This function will block until the application
    // recieves a WM_QUIT message.
    CefRunMessageLoop();
  } else {
    // Create a hidden window for message processing.
    hMessageWnd =  hexclient::CreateMessageWindow(hInstance);
    ASSERT(hMessageWnd);

    MSG msg;

    // Run the application message loop.
    while (GetMessage(&msg, NULL, 0, 0)) {
      if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }

    DestroyWindow(hMessageWnd);
    hMessageWnd = NULL;

    result = static_cast<int>(msg.wParam);
  }

  // Shut down CEF.
  CefShutdown();

  return result;
}
