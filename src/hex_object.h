// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#ifndef SRC_HEX_OBJECT_H
#define SRC_HEX_OBJECT_H

#include "hex_shared.h"
#include <v8.h>

namespace hex {

#define DECLARE_HEX_OBJECT_METHOD(name)                                       \
  void name##Callback(const v8::FunctionCallbackInfo<v8::Value>& args)
#define DECLARE_HEX_OBJECT_PROPERTY_GETTER(name)                              \
  void name##Getter(v8::Local<v8::String> property,                           \
                    const v8::PropertyCallbackInfo<v8::Value>& info);
#define DECLARE_HEX_OBJECT_PROPERTY_SETTER(name)                              \
  void name##Setter(v8::Local<v8::String> property,                           \
                    v8::Local<v8::Value> value,                               \
                    const v8::PropertyCallbackInfo<void>& info)

#define HEX_SYMBOL v8::String::NewSymbol("hex")


#if defined(OS_WIN)
DECLARE_HEX_OBJECT_PROPERTY_GETTER(WidgetWindowHandle);
DECLARE_HEX_OBJECT_PROPERTY_GETTER(RendererWindowHandle);
DECLARE_HEX_OBJECT_PROPERTY_GETTER(AeroGlassEnabled);

DECLARE_HEX_OBJECT_METHOD(Restore);
DECLARE_HEX_OBJECT_METHOD(Move);
DECLARE_HEX_OBJECT_METHOD(Size);
DECLARE_HEX_OBJECT_METHOD(Minimize);
DECLARE_HEX_OBJECT_METHOD(Maximize);
DECLARE_HEX_OBJECT_METHOD(Close);
DECLARE_HEX_OBJECT_METHOD(ShowSystemMenu);
DECLARE_HEX_OBJECT_METHOD(DeleteSystemCommand);
DECLARE_HEX_OBJECT_METHOD(EnableSystemCommand);
DECLARE_HEX_OBJECT_METHOD(InsertSystemCommand);
DECLARE_HEX_OBJECT_PROPERTY_GETTER(FormState);
DECLARE_HEX_OBJECT_PROPERTY_SETTER(FormState);
DECLARE_HEX_OBJECT_PROPERTY_GETTER(FormActivation);
DECLARE_HEX_OBJECT_METHOD(SetAsParentForm);
DECLARE_HEX_OBJECT_PROPERTY_GETTER(TopMost);
DECLARE_HEX_OBJECT_PROPERTY_SETTER(TopMost);
DECLARE_HEX_OBJECT_METHOD(SizeTo);
DECLARE_HEX_OBJECT_METHOD(MoveTo);
DECLARE_HEX_OBJECT_PROPERTY_GETTER(FormEnabled);
DECLARE_HEX_OBJECT_PROPERTY_SETTER(FormEnabled);
DECLARE_HEX_OBJECT_METHOD(FocusForm);
DECLARE_HEX_OBJECT_PROPERTY_GETTER(Transparency);
DECLARE_HEX_OBJECT_PROPERTY_SETTER(Transparency);
DECLARE_HEX_OBJECT_METHOD(SetAsTitleBarAreas);
DECLARE_HEX_OBJECT_METHOD(SetAsNonBorderAreas);
DECLARE_HEX_OBJECT_METHOD(SetAsSystemMenuIconAreas);
DECLARE_HEX_OBJECT_METHOD(SetPopupFormSize);
DECLARE_HEX_OBJECT_METHOD(SetPopupFormPosition);
DECLARE_HEX_OBJECT_PROPERTY_GETTER(MinWidth);
DECLARE_HEX_OBJECT_PROPERTY_SETTER(MinWidth);
DECLARE_HEX_OBJECT_PROPERTY_GETTER(MinHeight);
DECLARE_HEX_OBJECT_PROPERTY_SETTER(MinHeight);
DECLARE_HEX_OBJECT_PROPERTY_GETTER(MaxWidth);
DECLARE_HEX_OBJECT_PROPERTY_SETTER(MaxWidth);
DECLARE_HEX_OBJECT_PROPERTY_GETTER(MaxHeight);
DECLARE_HEX_OBJECT_PROPERTY_SETTER(MaxHeight);
DECLARE_HEX_OBJECT_METHOD(GetSize);
DECLARE_HEX_OBJECT_METHOD(GetPosition);
DECLARE_HEX_OBJECT_METHOD(GetWorkspaceRect);
DECLARE_HEX_OBJECT_METHOD(SetFormIcon);
DECLARE_HEX_OBJECT_METHOD(Terminate);
DECLARE_HEX_OBJECT_METHOD(CancelMode);
DECLARE_HEX_OBJECT_METHOD(GetMaximizedAdjustedBorderWidth);
#endif  // OS_WIN

HEX_EXPORT v8::Local<v8::Object> SetupHexObject(v8::Handle<v8::Object> global);

}  // namespace hex

#endif  // SRC_HEX_OBJECT_H
