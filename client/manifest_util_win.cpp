// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "manifest_util.h"
#include <Windows.h>
#include <atlconv.h>
#include <stdlib.h>

namespace hexclient {

std::wstring ManifestUtil::StyledString() {
  USES_CONVERSION;
  return A2W(root_.toStyledString().c_str());
}

bool ManifestUtil::ReadManifest() {
  if (file_name_.empty()) {
    return false;
  }
  USES_CONVERSION;
  HANDLE file = CreateFile(file_name_.c_str(),
                           GENERIC_READ,
                           FILE_SHARE_READ | FILE_SHARE_WRITE,
                           NULL,
                           OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL,
                           NULL);
  if (INVALID_HANDLE_VALUE == file) {
    CloseHandle(file);
    return false;
  }

  DWORD fileSize = GetFileSize(file, NULL);
  DWORD bytesRead;
  char* data = new char[fileSize + 1];
  ZeroMemory(data, sizeof *data);
  if (!ReadFile(file, data, fileSize, &bytesRead, NULL)) {
    CloseHandle(file);
    delete[] data;
    return false;
  }

  CefString buf = data;
  json_content_ = W2A(buf.c_str());

  delete[] data;
  CloseHandle(file);
  return true;
}

bool ManifestUtil::WriteManifest() {
  if (file_name_.empty()) {
    return false;
  }

  HANDLE file = CreateFile(file_name_.c_str(),
                           GENERIC_READ | GENERIC_WRITE,
                           FILE_SHARE_READ | FILE_SHARE_WRITE,
                           NULL,
                           CREATE_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL,
                           NULL);
  if (INVALID_HANDLE_VALUE == file) {
    CloseHandle(file);
    return false;
  }

  DWORD bytesWrite;
  if (!WriteFile(file, json_content_.c_str(), json_content_.size(),
      &bytesWrite, NULL)) {
    CloseHandle(file);
    return false;
  }

  CloseHandle(file);
  return true;
}

}  // namespace hexclient
