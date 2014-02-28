// Copyright (c) 2013 NetEase Youdao Inc. and other heX contributors.
// All rights reserved. Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "hex_use_node.h"
#include "hex_shared.h"
#if defined(OS_WIN)
#include <windows.h>
#include <string>
#endif  // OS_WIN

#define NODE_LIB_MODULE_NAME L"node"
#define UV_LIB_MODULE_NAME L"uv"
#if defined(OS_WIN)
#define LIB_MODULE_EXT L".dll"
#endif  // OS_WIN

namespace {

std::wstring GetApplicationDirectory() {
#if defined(OS_WIN)
  WCHAR dir[MAX_PATH];
  if (GetModuleFileName(NULL, dir, sizeof dir) == NULL) {
    dir[0] = 0;
  }
  WCHAR* pos = wcsrchr(dir, '\\');
  *pos = '\0';
#endif  // OS_WIN
  return dir;
}

bool PathExists(const std::wstring path) {
#if defined(OS_WIN)
  return (GetFileAttributes(path.c_str()) != INVALID_FILE_ATTRIBUTES);
#endif  // OS_WIN
}

}  // namespace

namespace hex {

bool IsNodeLibrariesExist() {
  std::wstring current_directory(GetApplicationDirectory());
  std::wstring node_lib_path(current_directory.append(L"/"));
  node_lib_path.append(NODE_LIB_MODULE_NAME);
#if defined(OS_WIN)
  node_lib_path.append(LIB_MODULE_EXT);
#endif  // OS_WIN
  std::wstring uv_lib_path(current_directory.append(L"/"));
  uv_lib_path.append(UV_LIB_MODULE_NAME);
#if defined(OS_WIN)
  uv_lib_path.append(LIB_MODULE_EXT);
#endif  // OS_WIN
  return PathExists(node_lib_path) && PathExists(uv_lib_path);
}

bool CanUseNode() {
  return IsNodeLibrariesExist();
}

}  // namespace hex
