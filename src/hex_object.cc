// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "hex_object.h"
#include "hex_switches_port.h"
#include "hex_shared.h"
#include "hex_use_node.h"
#if defined(OS_WIN)
#include "hex_shared_win.h"
#include <process.h>
#endif  // OS_WIN
#if defined(OS_MACOSX)
#include <unistd.h>
#endif  // OS_MACOSX


namespace hex {

#define HEX_SET_PROPERTY(obj, name, value, ...) \
  obj->Set(v8::String::NewSymbol(name), value, ##__VA_ARGS__)

#define HEX_SET_METHOD(obj, name, callback) \
  obj->Set(v8::String::NewSymbol(name),\
      v8::FunctionTemplate::New(callback)->GetFunction())

#define HEX_SET_ACCESSOR(obj, name, getter, ...) \
  obj->SetAccessor(v8::String::NewSymbol(name), getter, ##__VA_ARGS__)

#define METHOD(name) name##Callback

#define GETTER(name) name##Getter

#define SETTER(name) name##Setter

#define RODD static_cast<v8::PropertyAttribute>(v8::ReadOnly | v8::DontDelete)


v8::Local<v8::Object> SetupHexObject(v8::Handle<v8::Object> global) {
  v8::HandleScope scope;

  v8::Local<v8::FunctionTemplate> hex_template = v8::FunctionTemplate::New();
  hex_template->SetClassName(HEX_SYMBOL);
  v8::Local<v8::Object> hex_object = v8::Local<v8::Object>::New(
      hex_template->GetFunction()->NewInstance());

  int ppid = SingleProcess() ? 0 : GetMainProcessID();
  HEX_SET_PROPERTY(hex_object, "ppid", v8::Integer::New(ppid), RODD);
#if defined(OS_WIN)
  HWND main_window =
      GetMainWindowHandle(SingleProcess() ? _getpid() : ppid);
  HWND browser_window = GetBrowserWindowHandle(main_window,
      SingleProcess() ? _getpid() : ppid);
  bool is_main_window = IsMainWindow(browser_window, main_window);
  HWND top_window = is_main_window ? main_window : browser_window;

  HEX_SET_PROPERTY(hex_object, "mainWindowHandle",
      v8::Integer::New((int)main_window), RODD);
  HEX_SET_PROPERTY(hex_object, "browserWindowHandle",
      v8::Integer::New((int)browser_window), RODD);
  HEX_SET_PROPERTY(hex_object, "topWindowHandle",
      v8::Integer::New((int)top_window), RODD);
  HEX_SET_ACCESSOR(hex_object, "widgetWindowHandle",
      GETTER(WidgetWindowHandle));
  HEX_SET_ACCESSOR(hex_object, "rendererWindowHandle",
      GETTER(RendererWindowHandle));
  HEX_SET_ACCESSOR(hex_object, "aeroGlassEnabled", GETTER(AeroGlassEnabled));

  HEX_SET_PROPERTY(hex_object, "singleProcess",
      v8::Boolean::New(SingleProcess()), RODD);

  if (hex::CanUseNode()) {
    HEX_SET_PROPERTY(hex_object, "lockedPath", v8::String::New(LockedPath()),
        RODD);
    HEX_SET_PROPERTY(hex_object, "disabledNodeModules",
      v8::String::New(DisabledNodeModules()), RODD);
    HEX_SET_PROPERTY(hex_object, "enabledNodeModules",
      v8::String::New(EnabledNodeModules()), RODD);
    HEX_SET_PROPERTY(hex_object, "nodeThirdPartyNativeModulesDisabled",
      v8::Boolean::New(DisableNodeThirdPartyNativeModules()), RODD);
  }

  if (DisableFormApis()) {
    HEX_SET_PROPERTY(global, "hex", hex_object, RODD);
    return hex_object;
  }

  HEX_SET_PROPERTY(hex_object, "SYSTEMDEFAULT", v8::Int32::New(SYSTEMDEFAULT),
      RODD);
  HEX_SET_PROPERTY(hex_object, "PARENTCENTERED", v8::Int32::New(PARENTCENTERED),
      RODD);
  HEX_SET_PROPERTY(hex_object, "SCREENCENTERED", v8::Int32::New(SCREENCENTERED),
      RODD);
  HEX_SET_PROPERTY(hex_object, "ACTIVATED", v8::Int32::New(ACTIVATED), RODD);
  HEX_SET_PROPERTY(hex_object, "INACTIVATED", v8::Int32::New(INACTIVATED),
      RODD);
  HEX_SET_PROPERTY(hex_object, "NORMAL", v8::Int32::New(NORMAL), RODD);
  HEX_SET_PROPERTY(hex_object, "MINIMIZED", v8::Int32::New(MINIMIZED), RODD);
  HEX_SET_PROPERTY(hex_object, "MAXIMIZED", v8::Int32::New(MAXIMIZED), RODD);
  HEX_SET_PROPERTY(hex_object, "HIDDEN", v8::Int32::New(HIDDEN), RODD);
  HEX_SET_PROPERTY(hex_object, "FULLSCREEN", v8::Int32::New(FULLSCREEN), RODD);
  HEX_SET_PROPERTY(hex_object, "RESTORE", v8::Int32::New(RESTORE), RODD);
  HEX_SET_PROPERTY(hex_object, "MOVE", v8::Int32::New(MOVE), RODD);
  HEX_SET_PROPERTY(hex_object, "SIZE", v8::Int32::New(SIZE), RODD);
  HEX_SET_PROPERTY(hex_object, "MINIMIZE", v8::Int32::New(MINIMIZE), RODD);
  HEX_SET_PROPERTY(hex_object, "MAXIMIZE", v8::Int32::New(MAXIMIZE), RODD);
  HEX_SET_PROPERTY(hex_object, "CLOSE", v8::Int32::New(CLOSE), RODD);

  HEX_SET_PROPERTY(hex_object, "TOP", v8::Int32::New(TOP), RODD);
  HEX_SET_PROPERTY(hex_object, "TOPRIGHT", v8::Int32::New(TOPRIGHT), RODD);
  HEX_SET_PROPERTY(hex_object, "RIGHT", v8::Int32::New(RIGHT), RODD);
  HEX_SET_PROPERTY(hex_object, "BOTTOMRIGHT", v8::Int32::New(BOTTOMRIGHT),
      RODD);
  HEX_SET_PROPERTY(hex_object, "BOTTOM", v8::Int32::New(BOTTOM), RODD);
  HEX_SET_PROPERTY(hex_object, "BOTTOMLEFT", v8::Int32::New(BOTTOMLEFT), RODD);
  HEX_SET_PROPERTY(hex_object, "LEFT", v8::Int32::New(LEFT), RODD);
  HEX_SET_PROPERTY(hex_object, "TOPLEFT", v8::Int32::New(TOPLEFT), RODD);

  HEX_SET_METHOD(hex_object, "restore", METHOD(Restore));
  HEX_SET_METHOD(hex_object, "move", METHOD(Move));
  HEX_SET_METHOD(hex_object, "size", METHOD(Size));
  HEX_SET_METHOD(hex_object, "minimize", METHOD(Minimize));
  HEX_SET_METHOD(hex_object, "maximize", METHOD(Maximize));
  HEX_SET_METHOD(hex_object, "close", METHOD(Close));
  HEX_SET_METHOD(hex_object, "showSystemMenu", METHOD(ShowSystemMenu));
  HEX_SET_METHOD(hex_object, "deleteSystemCommand",
      METHOD(DeleteSystemCommand));
  HEX_SET_METHOD(hex_object, "enableSystemCommand",
      METHOD(EnableSystemCommand));
  HEX_SET_METHOD(hex_object, "insertSystemCommand",
      METHOD(InsertSystemCommand));
  HEX_SET_ACCESSOR(hex_object, "formState", GETTER(FormState),
      SETTER(FormState));
  HEX_SET_ACCESSOR(hex_object, "formActivation", GETTER(FormActivation));
  HEX_SET_METHOD(hex_object, "setAsParentForm", METHOD(SetAsParentForm));
  HEX_SET_ACCESSOR(hex_object, "topMost", GETTER(TopMost), SETTER(TopMost));
  HEX_SET_METHOD(hex_object, "sizeTo", METHOD(SizeTo));
  HEX_SET_METHOD(hex_object, "moveTo", METHOD(MoveTo));
  HEX_SET_ACCESSOR(hex_object, "formEnabled", GETTER(FormEnabled),
      SETTER(FormEnabled));
  HEX_SET_METHOD(hex_object, "focusForm", METHOD(FocusForm));
  HEX_SET_ACCESSOR(hex_object, "transparency", GETTER(Transparency),
      SETTER(Transparency));
  HEX_SET_METHOD(hex_object, "setAsTitleBarAreas",
      METHOD(SetAsTitleBarAreas));
  HEX_SET_METHOD(hex_object, "setAsNonBorderAreas",
      METHOD(SetAsNonBorderAreas));
  HEX_SET_METHOD(hex_object, "setAsSystemMenuIconAreas",
      METHOD(SetAsSystemMenuIconAreas));
  HEX_SET_METHOD(hex_object, "setPopupFormSize", METHOD(SetPopupFormSize));
  HEX_SET_METHOD(hex_object, "setPopupFormPosition",
      METHOD(SetPopupFormPosition));
  HEX_SET_ACCESSOR(hex_object, "minWidth", GETTER(MinWidth), SETTER(MinWidth));
  HEX_SET_ACCESSOR(hex_object, "minHeight", GETTER(MinHeight),
      SETTER(MinHeight));
  HEX_SET_ACCESSOR(hex_object, "maxWidth", GETTER(MaxWidth), SETTER(MaxWidth));
  HEX_SET_ACCESSOR(hex_object, "maxHeight", GETTER(MaxHeight),
      SETTER(MaxHeight));
  HEX_SET_METHOD(hex_object, "getSize", METHOD(GetSize));
  HEX_SET_METHOD(hex_object, "getPosition", METHOD(GetPosition));
  HEX_SET_PROPERTY(hex_object, "applicationDirectoryPath",
      v8::String::New(GetApplicationDirectoryPath().c_str()), RODD);
  HEX_SET_METHOD(hex_object, "getWorkspaceRect", METHOD(GetWorkspaceRect));
  HEX_SET_METHOD(hex_object, "setFormIcon", METHOD(SetFormIcon));
  HEX_SET_METHOD(hex_object, "terminate", METHOD(Terminate));
  HEX_SET_METHOD(hex_object, "cancelMode", METHOD(CancelMode));
  HEX_SET_METHOD(hex_object, "getMaximizedAdjustedBorderWidth",
      METHOD(GetMaximizedAdjustedBorderWidth));
#endif  // OS_WIN

  HEX_SET_PROPERTY(global, "hex", hex_object, RODD);

  return hex_object;
}

}  // namespace hex
