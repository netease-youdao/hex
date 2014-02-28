// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#ifndef HEX_SRC_NODE_CONSTRUCTORS_HOLDER_H_
#define HEX_SRC_NODE_CONSTRUCTORS_HOLDER_H_

#include <v8.h>
#include "hex_switches_port.h"

namespace node {

#define IS_SINGLE_PROCESS() hex::SingleProcess()

#define BEGIN_SINGLE_PROCESS_MODE() if (IS_SINGLE_PROCESS()) {
#define END_SINGLE_PROCESS_MODE() }
#define ELSE_SINGLE_PROCESS_MODE() } else {

#define ENTERED_GLOBAL v8::Context::GetEntered()->Global()

#define READONLY_DONTDELETE_DONTENUM static_cast<v8::PropertyAttribute>(v8::ReadOnly | v8::DontDelete | v8::DontEnum)

#define STRING_KEY(key) v8::String::NewSymbol(#key)

#define NODE_SET_CTOR(ctor_tmpl_func, name) BEGIN_SINGLE_PROCESS_MODE() \
  ENTERED_GLOBAL->ForceSet(STRING_KEY(name), ctor_tmpl_func, READONLY_DONTDELETE_DONTENUM); \
END_SINGLE_PROCESS_MODE();
#define NODE_SET_OBJ NODE_SET_CTOR

#define GET_CTOR(name) v8::Local<v8::Function>::Cast(ENTERED_GLOBAL->Get(STRING_KEY(name)))
#define NODE_GET_CTOR(name, ret, ...) BEGIN_SINGLE_PROCESS_MODE() \
  ret = GET_CTOR(name)->NewInstance(##__VA_ARGS__); \
END_SINGLE_PROCESS_MODE()
#define GET_OBJ(name) ENTERED_GLOBAL->Get(STRING_KEY(name))
#define NODE_GET_OBJ(name, ret) BEGIN_SINGLE_PROCESS_MODE() \
  ret = ENTERED_GLOBAL->Get(STRING_KEY(name)); \
END_SINGLE_PROCESS_MODE()

#define NODE_HAS_CTOR(name) ENTERED_GLOBAL->Has(STRING_KEY(name))
#define NODE_HAS_OBJ NODE_HAS_CTOR
#define BEGIN_NODE_HAS_CTOR(name) BEGIN_SINGLE_PROCESS_MODE() \
  if (NODE_HAS_CTOR(name)) {
#define END_NODE_HAS_CTOR() } END_SINGLE_PROCESS_MODE()
#define BEGIN_NODE_HAS_OBJ BEGIN_NODE_HAS_OBJ
#define END_NODE_HAS_OBJ END_NODE_HAS_CTOR

#define NODE_CTOR_SET(name, key, value, ...) BEGIN_SINGLE_PROCESS_MODE() \
  if (NODE_HAS_CTOR(name)) {\
    GET_CTOR(name)->Set(key, value, ##__VA_ARGS__);\
  }\
END_SINGLE_PROCESS_MODE()

#define NODE_HAS_INSTANCE(obj, name) obj->GetConstructor() == GET_CTOR(name)

}  // namespace node

#endif  // HEX_SRC_NODE_CONSTRUCTORS_HOLDER_H_
