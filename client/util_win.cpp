// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "util.h"
#include <Shlwapi.h>
#include <direct.h>
#include <atlconv.h>
#include <WinSock2.h>
#include <Iphlpapi.h>

namespace hexclient {

char workingDir[MAX_PATH];  // The current working directory
char executionDir[MAX_PATH];  // The execution directory

void InitUtil() {
  // Retrieve the current working directory.
  if (_getcwd(workingDir, MAX_PATH) == NULL)
    workingDir[0] = 0;
  if (GetModuleFileNameA(NULL, executionDir, sizeof executionDir) == NULL)
    executionDir[0] = 0;
  char* pos = strrchr(executionDir, '\\');
  *pos = '\0';
}

CefString GetFullPathInExecutionDirectory(CefString fileName,
                                          bool includingSlash) {
  WCHAR buf[MAX_PATH];
  ZeroMemory(buf, MAX_PATH);
  if (GetModuleFileName(NULL, buf, MAX_PATH)) {
    PathRemoveFileSpec(buf);
    if (includingSlash)
      wcscat_s(buf, _countof(buf), L"\\");
    if (!fileName.empty())
      wcscat_s(buf, _countof(buf), fileName.c_str());
  }
  return CefString(buf);
}

CefString GetExecutionDirectory() {
  return executionDir;
}

CefString GetWorkingDirectory() {
  return workingDir;
}

CefString ProcessAppDir(std::string filePath) {
  USES_CONVERSION;
  std::wstring path = A2W(filePath.c_str());
  std::wstring appDir = L"$(AppDir)";
  std::wstring::size_type pos = path.find(appDir);
  if (std::string::npos != pos) {
    path.replace(pos, appDir.size(), GetFullPathInExecutionDirectory("\\",
                                                                     false));
  }
  return path;
}

bool IsPortUsed(int port) {
  WSADATA wsadata;
  WSAStartup(MAKEWORD(2, 2), &wsadata);
  SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
  SOCKADDR_IN addr;
  DWORD res = 0;
  addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
  addr.sin_family = AF_INET;
  addr.sin_port = ntohs(port);
  int ret = bind(s, (SOCKADDR*)&addr, sizeof addr);
  int err = WSAGetLastError();
  //listen(s, 5);
  closesocket(s);
  WSACleanup();
  if (ret == SOCKET_ERROR) {
    return WSAEADDRINUSE == err;
  }
  return false;
}

}  // hexclient