// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "client_app.h"

#include <string>

#include "include/cef_cookie.h"
#include "include/cef_process_message.h"
#include "include/cef_task.h"
#include "include/cef_v8.h"
#include "util.h"
#include "manifest_util.h"

#define HEX_STRING L"hex"

namespace hexclient {

// Forward declarations.
void SetList(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target);
void SetList(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target);

// Transfer a V8 value to a List index.
void SetListValue(CefRefPtr<CefListValue> list, int index,
                  CefRefPtr<CefV8Value> value) {
  if (value->IsArray()) {
    CefRefPtr<CefListValue> new_list = CefListValue::Create();
    SetList(value, new_list);
    list->SetList(index, new_list);
  } else if (value->IsString()) {
    list->SetString(index, value->GetStringValue());
  } else if (value->IsBool()) {
    list->SetBool(index, value->GetBoolValue());
  } else if (value->IsInt()) {
    list->SetInt(index, value->GetIntValue());
  } else if (value->IsDouble()) {
    list->SetDouble(index, value->GetDoubleValue());
  }
}

// Transfer a V8 array to a List.
void SetList(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target) {
  ASSERT(source->IsArray());

  int arg_length = source->GetArrayLength();
  if (arg_length == 0)
    return;

  // Start with null types in all spaces.
  target->SetSize(arg_length);

  for (int i = 0; i < arg_length; ++i)
    SetListValue(target, i, source->GetValue(i));
}

// Transfer a List value to a V8 array index.
void SetListValue(CefRefPtr<CefV8Value> list, int index,
                  CefRefPtr<CefListValue> value) {
  CefRefPtr<CefV8Value> new_value;

  CefValueType type = value->GetType(index);
  switch (type) {
    case VTYPE_LIST: {
      CefRefPtr<CefListValue> list = value->GetList(index);
      new_value = CefV8Value::CreateArray(list->GetSize());
      SetList(list, new_value);
      } break;
    case VTYPE_BOOL:
      new_value = CefV8Value::CreateBool(value->GetBool(index));
      break;
    case VTYPE_DOUBLE:
      new_value = CefV8Value::CreateDouble(value->GetDouble(index));
      break;
    case VTYPE_INT:
      new_value = CefV8Value::CreateInt(value->GetInt(index));
      break;
    case VTYPE_STRING:
      new_value = CefV8Value::CreateString(value->GetString(index));
      break;
    default:
      break;
  }

  if (new_value.get()) {
    list->SetValue(index, new_value);
  } else {
    list->SetValue(index, CefV8Value::CreateNull());
  }
}

// Transfer a List to a V8 array.
void SetList(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target) {
  ASSERT(target->IsArray());

  int arg_length = source->GetSize();
  if (arg_length == 0)
    return;

  for (int i = 0; i < arg_length; ++i)
    SetListValue(target, i, source);
}


// Handles the native implementation for the methods of hex object.
class HeXMethodHandler : public CefV8Handler {
public:
  explicit HeXMethodHandler(CefRefPtr<CefFrame> frame)
     : frame_(frame) {
  }

  virtual bool Execute(const CefString& name,
                       CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& args,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception) {
    bool handled = false;

    if (name == "updateManifest") {
      // Update the current manifest information to the file.
      CefRefPtr<CefV8Value> global = frame_->GetV8Context()->GetGlobal();
      CefRefPtr<CefV8Value> hex_object = global->GetValue("hex");
      CefRefPtr<CefV8Value> manifest_object = hex_object->GetValue("manifest");
      CefRefPtr<CefV8Value> json_object = global->GetValue("JSON");
      CefRefPtr<CefV8Value> stringify_func = json_object->GetValue("stringify");
      CefV8ValueList stringify_func_args;
      stringify_func_args.push_back(manifest_object);
      stringify_func_args.push_back(CefV8Value::CreateNull());
      stringify_func_args.push_back(CefV8Value::CreateInt(2));
      CefRefPtr<CefV8Value> manifest_string =
          stringify_func->ExecuteFunction(global, stringify_func_args);
      CefString manifest_content = manifest_string->GetStringValue();
      ManifestUtil& manifest = ManifestUtil::Instance();
      if (!manifest.UpdateWithJson(manifest_content.ToString())) {
        exception = "Parse manifest json content error";
        retval = CefV8Value::CreateBool(false);
        return true;
      }
      if (!manifest.WriteManifest()) {
        exception = "Update manifest.json error";
        retval = CefV8Value::CreateBool(false);
        return true;
      }
      retval = CefV8Value::CreateBool(true);
      handled = true;
    }

    if (!handled)
      exception = "Invalid method arguments";

    return true;
  }

private:
  CefRefPtr<CefFrame> frame_;

  IMPLEMENT_REFCOUNTING(HeXMethodHandler);
};


ClientApp::ClientApp() {
  CreateBrowserDelegates(browser_delegates_);
  CreateRenderDelegates(render_delegates_);

  // Default schemes that support cookies.
  cookieable_schemes_.push_back("http");
  cookieable_schemes_.push_back("https");
}

void ClientApp::OnContextCreated(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 CefRefPtr<CefV8Context> context) {
  if (!context.get() || !frame.get()) {
    return;
  }
  CefRefPtr<CefV8Value> global = context->GetGlobal();
  if (!global.get() || !global->HasValue(HEX_STRING)) {
    return;
  }
  ManifestUtil& manifest = ManifestUtil::Instance();
  if (!manifest.ReadManifest()) {
    return;
  }
  if (!manifest.Parse()) {
    return;
  }
  CefRefPtr<CefV8Value> hex_object = global->GetValue("hex");
  std::wstring code(HEX_STRING);
  code.append(L".manifest=");
  code.append(manifest.StyledString());
  frame->ExecuteJavaScript(code, "", 0);

  CefRefPtr<CefV8Value> update_manifest_func =
      CefV8Value::CreateFunction("updateManifest", new HeXMethodHandler(frame));
  hex_object->SetValue("updateManifest", update_manifest_func,
      static_cast<CefV8Value::PropertyAttribute>(
          V8_PROPERTY_ATTRIBUTE_READONLY | V8_PROPERTY_ATTRIBUTE_DONTDELETE));
}

}  // namespace hexclient
