// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "hex_object.h"
#include "hex_switches_port.h"
#include "hex_shared_win.h"
#include <windowsx.h>
#include <TlHelp32.h>
#include <process.h>


namespace hex {

static int ppid = 0;
static HWND mainWindowHandle = NULL;

#define BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER(name)                      \
  void name##Getter(v8::Local<v8::String> property,                           \
                    const v8::PropertyCallbackInfo<v8::Value>& info) {        \
    v8::HandleScope scope;

#define END_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER() }

#define BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER(name)                      \
  void name##Setter(v8::Local<v8::String> property,                           \
                    v8::Local<v8::Value> value,                               \
                    const v8::PropertyCallbackInfo<void>& info) {             \
  v8::HandleScope scope;

#define END_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER() }

#define GET_TOP(from) (HWND)from.This()->Get(v8::String::NewSymbol(           \
  "topWindowHandle"))->Int32Value()

#define GET_BROWSER(from) (HWND)from.This()->Get(v8::String::NewSymbol(       \
  "browserWindowHandle"))->Int32Value()

#define GET_WIDGET(from) (HWND)from.This()->Get(v8::String::NewSymbol(        \
  "widgetWindowHandle"))->Int32Value()

#define GET_RENDERER(from) (HWND)from.This()->Get(v8::String::NewSymbol(      \
   "rendererWindowHandle"))->Int32Value()

#define REQUIRE_HEX_THIS()                                                    \
  if (args.Holder() != v8::Context::GetEntered()->Global()->Get(HEX_SYMBOL)) {\
    return v8::ThrowException(v8::Exception::TypeError(                       \
        v8::String::New("Illegal invocation")));                              \
  } 

#define BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(name)                               \
  void name##Callback(const v8::FunctionCallbackInfo<v8::Value>& args) {      \
                      v8::HandleScope scope;                                  \
    //REQUIRE_HEX_THIS();

#define END_IMPLEMENT_HEX_OBJECT_METHOD() }

#define RESULT(info, obj) info.GetReturnValue().Set(obj)

/*static void ChangeDirectory(v8::Handle<v8::String> dir) {
  v8::String::Utf8Value path(dir->ToString());
  uv_err_t r = uv_chdir(*path);
}*/


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER(WidgetWindowHandle)
  HWND widget = NULL;
  HWND browser = GET_BROWSER(info);
  widget = GetWidgetWindowHandle(browser);
  RESULT(info, v8::Integer::New((int)widget));
END_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER()


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER(RendererWindowHandle)
  HWND renderer = NULL;
  HWND browser = GET_BROWSER(info);
  HWND widget = GetWidgetWindowHandle(browser); 
  renderer = GetRendererWindowHandle(widget);
  RESULT(info, v8::Integer::New((int)renderer));
END_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER()


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER(AeroGlassEnabled)
  RESULT(info, v8::Boolean::New(IsAeroGlassEnabled()));
END_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(Restore)
  bool directly = false;
  if (args.Length() >= 1 && args[0]->IsBoolean()) {
    directly = args[0]->BooleanValue();
  }
  HWND top = GET_TOP(args);
  DoSystemCommand(top, RESTORE, directly);
  RESULT(args, v8::Undefined());
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(Move)
  bool directly = false;
  if (args.Length() >= 1 && args[0]->IsBoolean()) {
    directly = args[0]->BooleanValue();
  }
  HWND top = GET_TOP(args);
  DoSystemCommand(top, MOVE, directly);
  RESULT(args, v8::Undefined());
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(Size)
  bool directly = false;
  if (args.Length() >= 1 && args[0]->IsBoolean()) {
    directly = args[0]->BooleanValue();
  }
  HWND top = GET_TOP(args);
  DoSystemCommand(top, SIZE, directly);
  RESULT(args, v8::Undefined());
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(Minimize)
  bool directly = false;
  if (args.Length() >= 1 && args[0]->IsBoolean()) {
    directly = args[0]->BooleanValue();
  }
  HWND top = GET_TOP(args);
  DoSystemCommand(top, MINIMIZE, directly);
  RESULT(args, v8::Undefined());
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(Maximize)
  bool directly = false;
  if (args.Length() >= 1 && args[0]->IsBoolean()) {
    directly = args[0]->BooleanValue();
  }
  HWND top = GET_TOP(args);
  DoSystemCommand(top, MAXIMIZE, directly);
  RESULT(args, v8::Undefined());
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(Close)
  bool directly = false;
  if (args.Length() >= 1 && args[0]->IsBoolean()) {
    directly = args[0]->BooleanValue();
  }
  HWND top = GET_TOP(args);
  DoSystemCommand(top, CLOSE, directly);
  RESULT(args, v8::Undefined());
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(ShowSystemMenu)
  if (args.Length() < 2 || !args[0]->IsInt32() || !args[1]->IsInt32()) {
    RESULT(args, v8::Undefined());
    return;
  }
  HWND renderer = GET_RENDERER(args);
  int x = (int)args[0]->Int32Value();
  int y = (int)args[1]->Int32Value();
  DWORD returnCommand = TrackSystemMenu(renderer, x, y, NULL);
  RESULT(args, v8::Integer::New(returnCommand));
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(DeleteSystemCommand)
  if (args.Length() < 1 || !args[0]->IsInt32()) {
    RESULT(args, v8::False());
    return;
  }
  HWND top = GET_TOP(args);
  SystemCommand command = static_cast<SystemCommand>(args[0]->Int32Value());
  BOOL success = DeleteSystemCommand(top, command);
  RESULT(args, v8::Boolean::New(!!success));
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(EnableSystemCommand)
  if (args.Length() < 2 || !args[0]->IsInt32() || !args[1]->IsBoolean()) {
    RESULT(args, v8::Undefined());
    return;
  }
  HWND renderer = GET_RENDERER(args);
  SystemCommand command = static_cast<SystemCommand>(args[0]->Int32Value());
  bool enabled = args[1]->BooleanValue();
  BOOL prevState = EnableSystemCommand(renderer, command, enabled);
  RESULT(args, v8::Boolean::New(!!prevState));
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(InsertSystemCommand)
  if (args.Length() < 2 || !args[0]->IsInt32() || !args[1]->IsInt32()) {
    RESULT(args, v8::False());
    return;
  }
  HWND top = GET_TOP(args);
  SystemCommand insCommand = static_cast<SystemCommand>(args[0]->Int32Value());
  int command = args[1]->Int32Value();
  BOOL success = InsertSystemCommand(top, insCommand, command);
  RESULT(args, v8::Boolean::New(!!success));
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER(FormState)
  FormState state = NORMAL;
  HWND top = GET_TOP(info);
  state = GetState(top);
  RESULT(info, v8::Integer::New(static_cast<int>(state)));
END_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER();


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER(FormState)
  if (!value->IsInt32()) {
    return;
  }
  HWND top = GET_TOP(info);
  FormState state = static_cast<FormState>(value->Int32Value());
  SetState(top, state);
END_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER()


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER(FormActivation)
  bool activation = true;
  HWND top = GET_TOP(info);
  activation = IsFormActivated(top);
  RESULT(info, v8::Boolean::New(activation));
END_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER();


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(SetAsParentForm)
  if (args.Length() < 1 || !args[0]->IsBoolean()) {
    RESULT(args, v8::Undefined());
    return;
  }
  HWND browser = GET_BROWSER(args);
  bool modelDialog = args[0]->BooleanValue();
  SetAsParentForm(browser, modelDialog);
  RESULT(args, v8::Undefined());
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER(TopMost)
  HWND top = GET_TOP(info);
  BOOL topMost = IsTopMost(top);
  RESULT(info, v8::Boolean::New(!!topMost));
END_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER()


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER(TopMost)
  if (!value->IsBoolean()) {
    return;
  }
  HWND top = GET_TOP(info);
  bool topMost = value->BooleanValue();
  SetTopMost(top, topMost);
END_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(SizeTo)
  if (args.Length() < 2 || !args[0]->IsInt32() || !args[1]->IsInt32()) {
    RESULT(args, v8::Undefined());
    return;
  }
  bool sendMoving = true;
  if (args.Length() > 2 && args[2]->IsBoolean()) {
    sendMoving = args[2]->BooleanValue();
  }
  HWND top = GET_TOP(args);
  int x = (int)args[0]->Int32Value();
  int y = (int)args[1]->Int32Value();
  SIZEL size = SizeTo(top, x, y, sendMoving);
  v8::Handle<v8::Object> retObj = v8::Object::New();
  retObj->Set(v8::String::New("width"), v8::Int32::New(size.cx));
  retObj->Set(v8::String::New("height"), v8::Int32::New(size.cy));
  RESULT(args, retObj);
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(MoveTo)
  if (args.Length() < 2 || !args[0]->IsInt32() || !args[1]->IsInt32()) {
    RESULT(args, v8::Undefined());
    return;
  }
  bool sendMoving = true;
  if (args.Length() > 2 && args[2]->IsBoolean()) {
    sendMoving = args[2]->BooleanValue();
  }
  HWND top = GET_TOP(args);
  int x = (int)args[0]->Int32Value();
  int y = (int)args[1]->Int32Value();
  POINT point = MoveTo(top, x, y, sendMoving);
  v8::Handle<v8::Object> retObj = v8::Object::New();
  retObj->Set(v8::String::New("x"), v8::Int32::New(point.x));
  retObj->Set(v8::String::New("y"), v8::Int32::New(point.y));
  RESULT(args, retObj);
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER(FormEnabled)
  bool enabled = true;
  HWND top = GET_TOP(info);
  enabled = IsFormEnabled(top);
  RESULT(info, v8::Boolean::New(enabled));
END_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER();


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER(FormEnabled)
  HWND top = GET_TOP(info);
  HWND browser = GET_BROWSER(info);
  SetFormEnable(top, browser, (BOOL)value->BooleanValue());
END_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER();


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(FocusForm)
  HWND top = GET_TOP(args);
  BOOL success = FocusForm(top);
  RESULT(args, v8::Boolean::New(!!success));
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER(Transparency)
  HWND top = GET_TOP(info);
  int alpha = GetTransparency(top);
  if (alpha < 0) {
    RESULT(info, v8::Undefined());
    return;
  }
  RESULT(info, v8::Int32::New(alpha));
END_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER()


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER(Transparency)
  HWND top = GET_TOP(info);
  int alpha = (int)value->Int32Value();
  if (alpha >= 0 && alpha <= 255) {
    SetTransparency(top, alpha);
  }
END_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(SetAsTitleBarAreas)
  if (args.Length() < 2 || !args[0]->IsInt32() || !args[1]->IsInt32()) {
    RESULT(args, v8::Undefined());
    return;
  }
  HWND top = GET_TOP(args);
  HWND renderer = GET_RENDERER(args);
  int x = (int)args[0]->Int32Value();
  int y = (int)args[1]->Int32Value();
  SetAsTitleBarAreas(top, x, y);
  SetAsTitleBarAreas(renderer, x, y);
  RESULT(args, v8::Undefined());
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(SetAsNonBorderAreas)
  if (args.Length() < 2 || !args[0]->IsInt32() || !args[1]->IsInt32()) {
    RESULT(args, v8::Undefined());
    return;
  }
  HWND top = GET_TOP(args);
  HWND renderer = GET_RENDERER(args);
  int x = (int)args[0]->Int32Value();
  int y = (int)args[1]->Int32Value();
  SetAsNonBorderAreas(top, x, y);
  SetAsNonBorderAreas(renderer, x, y);
  RESULT(args, v8::Undefined());
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(SetAsSystemMenuIconAreas)
  if (args.Length() < 2 || !args[0]->IsInt32() || !args[1]->IsInt32()) {
    RESULT(args, v8::Undefined());
    return;
  }
  HWND top = GET_TOP(args);
  HWND renderer = GET_RENDERER(args);
  int x = (int)args[0]->Int32Value();
  int y = (int)args[1]->Int32Value();
  SetAsSystemMenuIconAreas(top, x, y);
  SetAsSystemMenuIconAreas(renderer, x, y);
  RESULT(args, v8::Undefined());
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(SetPopupFormSize)
  if (args.Length() < 2 || !args[0]->IsInt32() || !args[1]->IsInt32()) {
    RESULT(args, v8::Undefined());
    return;
  }
  HWND browser = GET_BROWSER(args);
  int width = (int)args[0]->Int32Value();
  int height = (int)args[1]->Int32Value();
  SetPopupFormSize(browser, width, height);
  RESULT(args, v8::Undefined());
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(SetPopupFormPosition)
  if (args.Length() < 2 || !args[0]->IsInt32() || !args[1]->IsInt32()) {
    RESULT(args, v8::Undefined());
    return;
  }
  HWND browser = GET_BROWSER(args);
  int x = (int)args[0]->Int32Value();
  int y = (int)args[1]->Int32Value();
  SetPopupFormPosition(browser, x, y);
  RESULT(args, v8::Undefined());
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER(MinWidth)
  HWND browser = GET_BROWSER(info);
  int minWidth = GetMinWidth(browser);
  RESULT(info, v8::Int32::New(minWidth));
END_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER();


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER(MinWidth)
  if (!value->IsInt32()) {
    return;
  }
  HWND browser = GET_BROWSER(info);
  int minWidth = value->Int32Value();
  SetMinWidth(browser, minWidth);
END_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER();


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER(MinHeight)
  HWND browser = GET_BROWSER(info);
  int minHeight = GetMinHeight(browser);
  RESULT(info, v8::Int32::New(minHeight));
END_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER();


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER(MinHeight)
  if (!value->IsInt32()) {
    return;
  }
  HWND browser = GET_BROWSER(info);
  int minHeight = value->Int32Value();
  SetMinHeight(browser, minHeight);
END_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER();


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER(MaxWidth)
  HWND browser = GET_BROWSER(info);
  int maxWidth = GetMaxWidth(browser);
  RESULT(info, v8::Int32::New(maxWidth));
END_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER();


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER(MaxWidth)
  if (!value->IsInt32()) {
    return;
  }
  HWND browser = GET_BROWSER(info);
  int maxWidth = value->Int32Value();
  SetMaxWidth(browser, maxWidth);
END_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER();


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER(MaxHeight)
  HWND browser = GET_BROWSER(info);
  int maxHeight = GetMaxHeight(browser);
  RESULT(info, v8::Int32::New(maxHeight));
END_IMPLEMENT_HEX_OBJECT_PROPERTY_GETTER();


BEGIN_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER(MaxHeight)
  if (!value->IsInt32()) {
    return;
  }
  HWND browser = GET_BROWSER(info);
  int maxHeight = value->Int32Value();
  SetMaxHeight(browser, maxHeight);
END_IMPLEMENT_HEX_OBJECT_PROPERTY_SETTER();


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(GetSize)
  HWND top = GET_TOP(args);
  SIZEL size = GetSize(top);
  v8::Handle<v8::Object> returnObject = v8::Object::New();
  returnObject->Set(v8::String::New("width"), v8::Int32::New(size.cx));
  returnObject->Set(v8::String::New("height"), v8::Int32::New(size.cy));
  RESULT(args, returnObject);
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(GetPosition)
  HWND top = GET_TOP(args);
  POINT point = GetPosition(top);
  v8::Handle<v8::Object> returnObject = v8::Object::New();
  returnObject->Set(v8::String::New("x"), v8::Int32::New(point.x));
  returnObject->Set(v8::String::New("y"), v8::Int32::New(point.y));
  RESULT(args, returnObject);
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(GetWorkspaceRect)
  HWND top = GET_TOP(args);
  RECT rect = GetWorkspaceRect(top);
  v8::Handle<v8::Object> returnObject = v8::Object::New();
  returnObject->Set(v8::String::New("left"), v8::Int32::New(rect.left));
  returnObject->Set(v8::String::New("top"), v8::Int32::New(rect.top));
  returnObject->Set(v8::String::New("right"), v8::Int32::New(rect.right));
  returnObject->Set(v8::String::New("bottom"), v8::Int32::New(rect.bottom));
  RESULT(args, returnObject);
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(SetFormIcon)
  if (args.Length() < 1 || !args[0]->IsString()) {
    RESULT(args, v8::False());
    return;
  }
  HWND top = GET_TOP(args);
  v8::Local<v8::String> path_str = args[0]->ToString();
  int len = path_str->Length();
  if (len == 0) {
    RESULT(args, v8::False());
    return;
  }
  WCHAR* path = new WCHAR[len + 1];
  path_str->Write(reinterpret_cast<uint16_t*>(path), 0, len + 1);
  bool result = SetFormIcon(top, path);
  delete [] path;
  RESULT(args, v8::Boolean::New(result));
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(Terminate)
  HWND top = GET_TOP(args);
  RESULT(args, v8::Boolean::New(Terminate(top)));
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(CancelMode)
  HWND top = GET_TOP(args);
  CancelMode(top);
END_IMPLEMENT_HEX_OBJECT_METHOD()


BEGIN_IMPLEMENT_HEX_OBJECT_METHOD(GetMaximizedAdjustedBorderWidth)
  HWND renderer = GET_RENDERER(args);
  RESULT(args, v8::Int32::New(GetMaximizedAdjustedBorderWidth(renderer)));
END_IMPLEMENT_HEX_OBJECT_METHOD()


}  // namespace hex
