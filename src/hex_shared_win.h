// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "hex_shared.h"
#include "hex_export.h"
#include <windows.h>

namespace hex {

#define WM_TRACKSYSTEMMENU                  WM_USER + 1000
#define WM_ENABLESYSTEMMENUITEM             WM_USER + 1001
#define WM_DELETESYSTEMMENUITEM             WM_USER + 1002
#define WM_SIZETO                           WM_USER + 1003
#define WM_MOVETO                           WM_USER + 1004
#define WM_SETASTITLEBARAREAS               WM_USER + 1005
#define WM_GETPOPUPFORMSIZE                 WM_USER + 1006
#define WM_SETPOPUPFORMSIZE                 WM_USER + 1007
#define WM_GETPOPUPFORMPOSITION             WM_USER + 1008
#define WM_SETPOPUPFORMPOSITION             WM_USER + 1009
#define WM_GETASPARENTFORM                  WM_USER + 1010
#define WM_SETASPARENTFORM                  WM_USER + 1011
#define WM_SETFORMENABLED                   WM_USER + 1012
#define WM_USESYSTEMANIMATION               WM_USER + 1013
#define WM_SHOWINTASKBAR                    WM_USER + 1014
#define WM_GETFORMPOSITION                  WM_USER + 1015
#define WM_GETFORMSIZE                      WM_USER + 1016
#define WM_REDRAWRENDERER                   WM_USER + 1017
#define WM_GETTRANSPARENCY                  WM_USER + 1019
#define WM_SETTRANSPARENCY                  WM_USER + 1020
#define WM_TRIGGERNONCLIENTAREA             WM_USER + 1021
#define WM_DOSYSCOMMANDDIRECTLY             WM_USER + 1022
#define WM_GETWORKSPACERECT                 WM_USER + 1023
#define WM_GETMINWIDTH                      WM_USER + 1030
#define WM_SETMINWIDTH                      WM_USER + 1031
#define WM_GETMAXWIDTH                      WM_USER + 1032
#define WM_SETMAXWIDTH                      WM_USER + 1033
#define WM_GETMINHEIGHT                     WM_USER + 1034
#define WM_SETMINHEIGHT                     WM_USER + 1035
#define WM_GETMAXHEIGHT                     WM_USER + 1036
#define WM_SETMAXHEIGHT                     WM_USER + 1037
#define WM_DESTROYMAIN                      WM_USER + 1038
#define WM_SETASNONBORDERAREAS              WM_USER + 1039
#define WM_SETASSYSTEMMENUICONAREAS         WM_USER + 1040
#define WM_GETMAXIMIZEDADJUSTEDBORDERWIDTH  WM_USER + 1041


HEX_EXPORT LPCTSTR GetMainWindowClassName(int processId);
HEX_EXPORT LPCTSTR GetBrowserWindowClassName(int processId);
HEX_EXPORT HWND GetMainWindowHandle(DWORD processId);
HEX_EXPORT HWND GetBrowserWindowHandle(HWND main, int processId);
HEX_EXPORT HWND GetWidgetWindowHandle(HWND browser);
HEX_EXPORT HWND GetRendererWindowHandle(HWND widget);
HEX_EXPORT bool IsMainWindow(HWND handle, HWND parent);
HEX_EXPORT std::wstring UnderCurrentDirectory(LPCTSTR pathOrFile);
HEX_EXPORT BOOL DoSystemCommand(HWND handle, SystemCommand systemCommand,
    bool directly);
HEX_EXPORT DWORD TrackSystemMenu(HWND handle, int x, int y, HWND owner);
HEX_EXPORT BOOL DeleteSystemCommand(HWND handle, SystemCommand command);
HEX_EXPORT bool EnableSystemCommand(HWND handle, SystemCommand command,
    bool enabled);
HEX_EXPORT BOOL InsertSystemCommand(HWND handle, SystemCommand command,
    int pos);
HEX_EXPORT FormState GetState(HWND handle);
HEX_EXPORT void SetState(HWND handle, FormState state, bool firstSet = false);
HEX_EXPORT bool IsFormActivated(HWND handle);
HEX_EXPORT void SetAsParentForm(HWND handle, bool parent);
HEX_EXPORT BOOL SetTopMost(HWND handle, bool topMost);
HEX_EXPORT BOOL IsTopMost(HWND handle);
HEX_EXPORT SIZEL SizeTo(HWND handle, int width, int height, bool sendMoving);
HEX_EXPORT POINT MoveTo(HWND handle, int x, int y, bool sendMoving);
HEX_EXPORT bool IsFormEnabled(HWND handle);
HEX_EXPORT void SetFormEnable(HWND handle, HWND browser, BOOL enabled);
HEX_EXPORT BOOL FocusForm(HWND handle);
HEX_EXPORT bool SetTransparency(HWND handle, BYTE alpha);
HEX_EXPORT int GetTransparency(HWND handle);
HEX_EXPORT void SetAsTitleBarAreas(HWND handle, int x, int y);
HEX_EXPORT void SetAsNonBorderAreas(HWND handle, int x, int y);
HEX_EXPORT void SetAsSystemMenuIconAreas(HWND handle, int x, int y);
HEX_EXPORT void SetPopupFormSize(HWND handle, int width, int height);
HEX_EXPORT void SetPopupFormPosition(HWND handle, int x, int y);
HEX_EXPORT int GetMinWidth(HWND handle);
HEX_EXPORT void SetMinWidth(HWND handle, int minWidth);
HEX_EXPORT int GetMinHeight(HWND handle);
HEX_EXPORT void SetMinHeight(HWND handle, int minHeight);
HEX_EXPORT int GetMaxWidth(HWND handle);
HEX_EXPORT void SetMaxWidth(HWND handle, int maxWidth);
HEX_EXPORT int GetMaxHeight(HWND handle);
HEX_EXPORT void SetMaxHeight(HWND handle, int maxHeight);
HEX_EXPORT SIZEL GetSize(HWND handle);
HEX_EXPORT POINT GetPosition(HWND handle);
HEX_EXPORT RECT GetWorkspaceRect(HWND handle);
HEX_EXPORT bool SetFormIcon(HWND handle, WCHAR* path);
HEX_EXPORT MONITORINFO GetMonitorInformation(HWND handle);
HEX_EXPORT bool Terminate(HWND handle);

HEX_EXPORT void CancelMode(HWND handle);

HEX_EXPORT int GetMaximizedAdjustedBorderWidth(HWND handle);

// Determine if the current OS is Windows XP.
HEX_EXPORT bool IsWindowsXP();

// Determine if the aero glass can be used in the current OS.
HEX_EXPORT bool IsAeroGlassEnabled();

// Apply the aero glass to extend the whole window.
HEX_EXPORT void SetAeroGlass(HWND handle, bool enable_on_maximized);

}  // namespace hex
