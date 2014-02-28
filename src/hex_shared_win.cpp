// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "hex_shared.h"
#include "hex_shared_win.h"
#include <TlHelp32.h>
#include <process.h>
#include <shlwapi.h>
#include <tchar.h>
#include <windowsx.h>
#include <atlconv.h>
#include <map>
#include <dwmapi.h>

namespace hex {

std::map<SystemCommand, MENUITEMINFO> menuItemInfoMap =
    std::map<SystemCommand, MENUITEMINFO>();
std::map<SystemCommand, std::wstring> menuItemStringMap =
    std::map<SystemCommand, std::wstring>();


int GetMainProcessID() {
  DWORD dwCurPid = _getpid();
  DWORD dwMainPid = 0;
  HANDLE hProcessSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 pe32 = { 0 };
  pe32.dwSize = sizeof pe32;
  BOOL bRet = FALSE;
  bRet = Process32First(hProcessSnapShot, &pe32);
  if (!bRet) {
    return dwMainPid;
  }
  do {
    if (dwCurPid == pe32.th32ProcessID) {
      dwMainPid = pe32.th32ParentProcessID;
      break;
    }
  }
  while (Process32Next(hProcessSnapShot, &pe32));
  CloseHandle(hProcessSnapShot);
  return dwMainPid;
}


void GenerateSystemCommandMap(HWND handle) {
  HMENU systemMenu = GetSystemMenu(handle, FALSE);
  if (!systemMenu)
    return;
  for (int i = 0; i < GetMenuItemCount(systemMenu); i++) {
    UINT id = GetMenuItemID(systemMenu, i);
    MENUITEMINFO mii;
    ZeroMemory(&mii, sizeof mii);
    mii.cbSize = sizeof mii;
    mii.fMask = MIIM_ID | MIIM_STRING | MIIM_BITMAP;
    WCHAR data[MAX_PATH];
    mii.dwTypeData = data;
    mii.cch = _countof(data);

    switch (id) {
      case SC_RESTORE:
        GetMenuItemInfo(systemMenu, id, FALSE, &mii);
        menuItemInfoMap[RESTORE] = mii;
        menuItemStringMap[RESTORE] = mii.dwTypeData;
        break;
      case SC_MAXIMIZE:
        GetMenuItemInfo(systemMenu, id, FALSE, &mii);
        menuItemInfoMap[MAXIMIZE] = mii;
        menuItemStringMap[MAXIMIZE] = mii.dwTypeData;
        break;
      case SC_MINIMIZE:
        GetMenuItemInfo(systemMenu, id, FALSE, &mii);
        menuItemInfoMap[MINIMIZE] = mii;
        menuItemStringMap[MINIMIZE] = mii.dwTypeData;
        break;
      case SC_MOVE:
        GetMenuItemInfo(systemMenu, id, FALSE, &mii);
        menuItemInfoMap[MOVE] = mii;
        menuItemStringMap[MOVE] = mii.dwTypeData;
        break;
      case SC_SIZE:
        GetMenuItemInfo(systemMenu, id, FALSE, &mii);
        menuItemInfoMap[SIZE] = mii;
        menuItemStringMap[SIZE] = mii.dwTypeData;
        break;
      case SC_CLOSE:
        GetMenuItemInfo(systemMenu, id, FALSE, &mii);
        menuItemInfoMap[CLOSE] = mii;
        menuItemStringMap[CLOSE] = mii.dwTypeData;
        break;
      default:
        break;
    }
  }
}

HWND seekedHandle = NULL;

BOOL CALLBACK EnumChildBrowserProc(HWND handle, LPARAM lParam) {
  if (!FindWindowEx(handle, NULL, NULL, NULL)) {
    return TRUE;
  }
  EnumChildWindows(handle, EnumChildBrowserProc, lParam);
  WCHAR className[MAX_PATH];
  ZeroMemory(className, sizeof className);
  GetClassName(handle, className, sizeof className);
  WCHAR baseClassName[] = L"CefBrowserWindow";
  if (wcsstr(className, baseClassName)) {
    seekedHandle = handle;
    return FALSE;
  }
  return TRUE;
}


HWND GetMainWindowHandle(DWORD processId) {
  if (!HeXModule()/* && !DesktopWidget()*/) {
    return FindWindow(GetMainWindowClassName(processId), NULL);
  }

  /*if (DesktopWidget()) {
    HWND desktop = FindWindow(L"Progman", NULL);
    desktop = GetWindow(desktop, GW_CHILD);
    HWND main_window = FindWindowEx(desktop, NULL,
        GetMainWindowClassName(processId), NULL);
    return main_window;
  }*/

  seekedHandle = NULL;  
  HWND topWindow = GetTopWindow(NULL);
  while (topWindow){
    DWORD pid = 0;
    DWORD threadId = GetWindowThreadProcessId(topWindow, &pid);
    if (threadId != 0 && pid == processId) {
      EnumChildWindows(topWindow, EnumChildBrowserProc, (LPARAM)pid);
      if (seekedHandle) {
        return GetAncestor(seekedHandle, GA_ROOT);
      }
    }
    topWindow = GetNextWindow(topWindow, GW_HWNDNEXT);
  }
  return NULL;
}

LPCTSTR GetBrowserWindowClassName(int processId) {
  static WCHAR className[MAX_PATH];
  ZeroMemory(className, MAX_PATH);
  wsprintf(className, _T("CefBrowserWindow%X"), processId);
  return className;
}

LPCTSTR GetMainWindowClassName(int processId) {
  static WCHAR className[MAX_PATH];
  ZeroMemory(className, MAX_PATH);
  wsprintf(className, _T("HEXCLIENT%X"), processId);
  return className;
}

HWND GetBrowserWindowHandle(HWND mainWindowHandle, int processId) {
  LPCTSTR className = GetBrowserWindowClassName(processId);
  HWND hWnd = FindWindow(className, NULL);
  if (hWnd == NULL) {
    hWnd = FindWindowEx(mainWindowHandle, NULL, className, NULL);
  }
  return hWnd;
}

HWND GetWidgetWindowHandle(HWND browserWindowHandle) {
  LPCTSTR lpszClassName = L"Chrome_WidgetWin_0";
  HWND hWnd = FindWindowEx(browserWindowHandle, NULL, lpszClassName, NULL);
  return hWnd;
}

HWND GetRendererWindowHandle(HWND widgetWindowHandle) {
  LPCTSTR lpszClassName = L"Chrome_RenderWidgetHostHWND";
  HWND hWnd = FindWindowEx(widgetWindowHandle, NULL, lpszClassName, NULL);
  return hWnd;
}

bool IsMainWindow(HWND handle, HWND parent) {
  return parent == GetParent(handle);
}

std::wstring UnderCurrentDirectory(LPCTSTR pathOrFile) {
  WCHAR buf[MAX_PATH];
  ZeroMemory(buf, MAX_PATH);
  if (GetModuleFileName(NULL, buf, MAX_PATH)) {
    PathRemoveFileSpec(buf);
    wcscat_s(buf, _countof(buf), _T("\\"));
    wcscat_s(buf, _countof(buf), pathOrFile);
  }
  return buf;
}

BOOL DoSystemCommand(HWND hWnd, SystemCommand systemCommand, bool directly) {
  LONG sysCmd = 0;
  switch (systemCommand) {
    case RESTORE:
      sysCmd = SC_RESTORE;
      break;
    case MOVE:
      sysCmd = SC_MOVE;
      break;
    case SIZE:
      sysCmd = SC_SIZE;
      break;
    case MINIMIZE:
      sysCmd = SC_MINIMIZE;
      break;
    case MAXIMIZE:
      sysCmd = SC_MAXIMIZE;
      break;
    case CLOSE:
      sysCmd = SC_CLOSE;
      break;
  }
  SendMessage(hWnd, WM_DOSYSCOMMANDDIRECTLY, NULL, (LPARAM)directly);
  return SendMessage(hWnd, WM_SYSCOMMAND, (WPARAM)sysCmd, NULL);
}

DWORD TrackSystemMenu(HWND hWnd, int x, int y, HWND owner) {
  POINT point;
  point.x = x;
  point.y = y;
  ClientToScreen(hWnd, &point);
  return SendMessage(hWnd, WM_TRACKSYSTEMMENU, (WPARAM)hWnd,
      MAKELPARAM(point.x, point.y));
}


BOOL DeleteSystemCommand(HWND handle, SystemCommand command) {
  HMENU systemMenu = GetSystemMenu(handle, FALSE);
  if (!systemMenu)
    return FALSE;
  UINT flags = MF_BYCOMMAND;
  switch (command) {
    case MOVE:
      return DeleteMenu(systemMenu, SC_MOVE, flags);
    case SIZE:
      return DeleteMenu(systemMenu, SC_SIZE, flags);
    case RESTORE:
      return DeleteMenu(systemMenu, SC_RESTORE, flags);
    case MINIMIZE:
      return DeleteMenu(systemMenu, SC_MINIMIZE, flags);
    case MAXIMIZE:
      return DeleteMenu(systemMenu, SC_MAXIMIZE, flags);
    case CLOSE:
      return DeleteMenu(systemMenu, SC_CLOSE, flags);
  }
  return FALSE;
}


bool EnableSystemCommand(HWND handle, SystemCommand command, bool enabled) {
  HMENU systemMenu = GetSystemMenu(handle, FALSE);
  if (!systemMenu)
    return false;
  UINT item = 0;
  switch (command) {
    case MOVE:
      item = SC_MOVE;
      break;
    case SIZE:
      item = SC_SIZE;
      break;
    case RESTORE:
      item = SC_RESTORE;
      break;
    case MINIMIZE:
      item = SC_MINIMIZE;
      break;
    case MAXIMIZE:
      item = SC_MAXIMIZE;
      break;
    case CLOSE:
      item = SC_CLOSE;
      break;
  }
  if (item != 0) {
    return (bool)SendMessage(handle, WM_ENABLESYSTEMMENUITEM, item, enabled);
  }
  return false;
}

BOOL InsertSystemCommand(HWND handle, SystemCommand command, int pos) {
  HMENU systemMenu = GetSystemMenu(handle, FALSE);
  if (!systemMenu)
    return FALSE;
  UINT flags = pos >= RESTORE ? MF_BYCOMMAND : MF_BYPOSITION;
  if (pos >= RESTORE) {
    SystemCommand insCommand = static_cast<SystemCommand>(pos);
    switch (insCommand) {
      case MOVE:
        pos = SC_MOVE;
        break;
      case SIZE:
        pos = SC_SIZE;
        break;
      case RESTORE:
        pos = SC_RESTORE;
        break;
      case MINIMIZE:
        pos = SC_MINIMIZE;
        break;
      case MAXIMIZE:
        pos = SC_MAXIMIZE;
        break;
      case CLOSE:
        pos = SC_CLOSE;
        break;
    }
  }
  menuItemInfoMap[command].dwTypeData =
      (LPWSTR)menuItemStringMap[command].c_str();
  menuItemInfoMap[command].cch = menuItemStringMap[command].size();
  return InsertMenuItem(systemMenu, pos, flags, &(menuItemInfoMap[command]));
}


FormState GetState(HWND handle) {
  FormState state = NORMAL;
  WINDOWPLACEMENT wp;
  ZeroMemory(&wp, sizeof wp);
  wp.length = sizeof wp;
  GetWindowPlacement(handle, &wp);
  switch (wp.showCmd) {
    case SW_SHOWMINIMIZED:
      state = MINIMIZED;
      break;
    case SW_SHOWMAXIMIZED:
      state = MAXIMIZED;
      break;
    default:
      state = NORMAL;
      break;
  }
  if (state != MAXIMIZED) {
    MONITORINFO mi = GetMonitorInformation(handle);
    RECT rect;
    GetWindowRect(handle, &rect);
    if (mi.rcMonitor.top == rect.top && mi.rcMonitor.right == rect.right &&
        mi.rcMonitor.bottom == rect.bottom && mi.rcMonitor.left == rect.left) {
      state = FULLSCREEN;
    }
  }
  return state;
}


void SetState(HWND handle, FormState state, bool firstSet) {
  FormState previousState = GetState(handle);
  if (!firstSet && (previousState == FULLSCREEN || state == FULLSCREEN))
    return;
  WINDOWPLACEMENT wp;
  ZeroMemory(&wp, sizeof wp);
  wp.length = sizeof wp;
  GetWindowPlacement(handle, &wp);
  int sizeState = SIZE_RESTORED;
  switch (state) {
    case MINIMIZED:
      wp.showCmd = SW_SHOWMINIMIZED;
      sizeState = SIZE_MINIMIZED;
      break;
    case MAXIMIZED:
      wp.showCmd = SW_SHOWMAXIMIZED;
      sizeState = SIZE_MAXIMIZED;
      break;
    case NORMAL:
    default:
      wp.showCmd = SW_SHOWNORMAL;
      sizeState = SIZE_RESTORED;
      break;
  }
  if (state == FULLSCREEN) {
    MONITORINFO mi = GetMonitorInformation(handle);
    SetWindowPos(handle, NULL, mi.rcMonitor.left, mi.rcMonitor.top,
        mi.rcMonitor.right, mi.rcMonitor.bottom, SWP_SHOWWINDOW);
    sizeState = SIZE_MAXIMIZED;
  } else {
    ShowWindow(handle, wp.showCmd);
    UpdateWindow(handle);
  }
  PostMessage(handle, WM_SIZE, sizeState, NULL);
}


bool IsFormActivated(HWND handle) {
  return handle == GetActiveWindow();
}


void SetAsParentForm(HWND handle, bool parentForm) {
  SendMessage(handle, WM_SETASPARENTFORM, (WPARAM)parentForm, NULL);
}


BOOL SetTopMost(HWND handle, bool topMost) {
  return SetWindowPos(handle, topMost ? HWND_TOPMOST : HWND_NOTOPMOST,
      0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE);
}


BOOL IsTopMost(HWND handle) {
  return GetWindowLong(handle, GWL_EXSTYLE) & WS_EX_TOPMOST;
}


SIZEL SizeTo(HWND handle, int width, int height, bool sendMoving) {
  LRESULT ret = SendMessage(handle, WM_SIZETO, (WPARAM)sendMoving, MAKELPARAM(width, height));
  SIZEL size = { GET_X_LPARAM(ret), GET_Y_LPARAM(ret) };
  return size;
}


POINT MoveTo(HWND handle, int x, int y, bool sendMoving) {
  LRESULT ret = SendMessage(handle, WM_MOVETO, (WPARAM)sendMoving, MAKELPARAM(x, y));
  POINT point = { GET_X_LPARAM(ret), GET_Y_LPARAM(ret) };
  return point;
}


bool IsFormEnabled(HWND handle) {
  return IsWindowEnabled(handle);
}


void SetFormEnable(HWND handle, HWND browser, BOOL enabled) {
  EnableWindow(handle, enabled);
}


BOOL FocusForm(HWND handle) {
  return SetForegroundWindow(handle);
}


bool SetTransparency(HWND handle, BYTE alpha) {
  return !!SendMessage(handle, WM_SETTRANSPARENCY, alpha, NULL);
}


int GetTransparency(HWND handle) {
  return (int)SendMessage(handle, WM_GETTRANSPARENCY, NULL, NULL);
}


void SetAsTitleBarAreas(HWND handle, int x, int y) {
  SendMessage(handle, WM_SETASTITLEBARAREAS, x, y);
}


void SetAsNonBorderAreas(HWND handle, int x, int y) {
  SendMessage(handle, WM_SETASNONBORDERAREAS, x, y);
}


void SetAsSystemMenuIconAreas(HWND handle, int x, int y) {
  SendMessage(handle, WM_SETASSYSTEMMENUICONAREAS, x, y);
}


void SetPopupFormSize(HWND handle, int width, int height) {
  SendMessage(handle, WM_SETPOPUPFORMSIZE, NULL, MAKELPARAM(width, height));
}


void SetPopupFormPosition(HWND handle, int x, int y) {
  SendMessage(handle, WM_SETPOPUPFORMPOSITION, NULL, MAKELPARAM(x, y));
}


int GetMinWidth(HWND handle) {
  return (int)SendMessage(handle, WM_GETMINWIDTH, NULL, NULL);
}


void SetMinWidth(HWND handle, int minWidth) {
  SendMessage(handle, WM_SETMINWIDTH, NULL, (LPARAM)minWidth);
}


int GetMinHeight(HWND handle) {
  return (int)SendMessage(handle, WM_GETMINHEIGHT, NULL, NULL);
}


void SetMinHeight(HWND handle, int minHeight) {
  SendMessage(handle, WM_SETMINHEIGHT, NULL, (LPARAM)minHeight);
}


int GetMaxWidth(HWND handle) {
  return (int)SendMessage(handle, WM_GETMAXWIDTH, NULL, NULL);
}


void SetMaxWidth(HWND handle, int maxWidth) {
  SendMessage(handle, WM_SETMAXWIDTH, NULL, (LPARAM)maxWidth);
}


int GetMaxHeight(HWND handle) {
  return (int)SendMessage(handle, WM_GETMAXHEIGHT, NULL, NULL);
}


void SetMaxHeight(HWND handle, int maxHeight) {
  SendMessage(handle, WM_SETMAXHEIGHT, NULL, (LPARAM)maxHeight);
}

SIZEL GetSize(HWND handle) {
  LRESULT ret = SendMessage(handle, WM_GETFORMSIZE, NULL, NULL);
  SIZEL size = { GET_X_LPARAM(ret), GET_Y_LPARAM(ret) };
  return size;
}

POINT GetPosition(HWND handle) {
  LRESULT ret = SendMessage(handle, WM_GETFORMPOSITION, NULL, NULL);
  POINT point = { GET_X_LPARAM(ret), GET_Y_LPARAM(ret) };
  return point;
}


std::string GetApplicationDirectoryPath() {
  char dir[MAX_PATH];
  if (GetModuleFileNameA(NULL, dir, sizeof dir) == NULL) {
    dir[0] = 0;
  }
  char* pos = strrchr(dir, '\\');
  *pos = '\0';
  return dir;
}

std::wstring GetApplicationDirectoryPathW() {
  USES_CONVERSION;
  return A2W(GetApplicationDirectoryPath().c_str());
}


RECT GetWorkspaceRect(HWND handle) {
  MONITORINFO mi = GetMonitorInformation(handle);
  return mi.rcWork;
}


bool SetFormIcon(HWND handle, WCHAR* path) {
  if (!handle || !path) {
    return false;
  }
  HWND icon_handle = (HWND)LoadImage(NULL, path, IMAGE_ICON, 0, 0,
      LR_LOADFROMFILE | LR_DEFAULTSIZE);
  if (!icon_handle) {
    return false;
  }
  SetClassLong(handle, GCL_HICON, (LONG)icon_handle);
  SetClassLong(handle, GCL_HICONSM, (LONG)icon_handle);
  SendMessage(handle, WM_SETICON, ICON_BIG, (LPARAM)icon_handle);
  SendMessage(handle, WM_SETICON, ICON_SMALL, (LPARAM)icon_handle);
  SendMessage(handle, WM_SETICON, ICON_SMALL2, (LPARAM)icon_handle);      
  return true;
}


bool IsFormActivation(HWND hWnd) {
  return hWnd == GetActiveWindow();
}

MONITORINFO GetMonitorInformation(HWND handle) {
  HMONITOR monitor;
  MONITORINFO mi;
  RECT rect;
  GetWindowRect(handle, &rect);
  monitor = MonitorFromRect(&rect, MONITOR_DEFAULTTONEAREST);
  mi.cbSize = sizeof mi;
  GetMonitorInfo(monitor, &mi);
  return mi;
}


bool Terminate(HWND handle) {
  return SendMessage(handle, WM_DESTROYMAIN, NULL, NULL);
}


void CancelMode(HWND handle) {
  SendMessage(handle, WM_CANCELMODE, NULL, NULL);
}


int GetMaximizedAdjustedBorderWidth(HWND handle) {
  return SendMessage(handle, WM_GETMAXIMIZEDADJUSTEDBORDERWIDTH, NULL, NULL);
}


bool IsWindowsXP() {
  OSVERSIONINFO osversion;
  ZeroMemory(&osversion, sizeof osversion);
  osversion.dwOSVersionInfoSize = sizeof osversion;
  GetVersionEx(&osversion);
  return osversion.dwMajorVersion < 6;
}

bool IsAeroGlassEnabled() {
  if (IsWindowsXP()) {
    return false;
  }

  BOOL enabled = FALSE;
  return SUCCEEDED(DwmIsCompositionEnabled(&enabled)) && enabled;
}

void SetAeroGlass(HWND handle, bool enable_on_maximized) {
  if (!IsAeroGlassEnabled()) {
    return;
  }
  MARGINS margin = { -1, -1, -1, -1 };
  DwmExtendFrameIntoClientArea(handle, &margin);
  if (enable_on_maximized) {
    DWM_BLURBEHIND blurBehind;
    blurBehind.dwFlags = DWM_BB_ENABLE | DWM_BB_TRANSITIONONMAXIMIZED;
    blurBehind.fEnable = true;
    blurBehind.fTransitionOnMaximized = true;
    DwmEnableBlurBehindWindow(handle, &blurBehind);
  }
}


}  // namespace hex