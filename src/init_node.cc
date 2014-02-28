// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#include "init_node.h"

namespace hex {

static inline const CommandLine& CL() {
  return *CommandLine::ForCurrentProcess();
}

void ProcessNodeArguments(std::vector<std::string> args) {
  int argc;
  char** argv;
  argc = args.size();
  size_t strlen_sum = 0;
  std::vector<std::string>::const_iterator it;
  for(it = args.begin(); it != args.end(); it++) {
    strlen_sum += it->length() + 1;
  }
  argv = (char**)malloc(sizeof(char*) * (argc + 1) + strlen_sum);
  if (!argv) {
    return;
  }
  char* data = (char*)argv + sizeof(char*) * (argc + 1);
  size_t len = 0;
  int i = 0;
  for (it = args.begin(); it != args.end(); it++, i++) {
    argv[i] = data;
    len = it->length() + 1;
    memcpy(data, it->c_str(), len);
    data += len;
  }
  argv[argc] = NULL;
  node::SetArguments(argc, argv);
}


void InitializeNode(std::vector<std::string> args) {
  ProcessNodeArguments(args);
#if defined(HEXCLIENT)
  hex::SetSingleProcess(CL().HasSwitch(switches::kSingleProcess));
  hex::SetLockedPath(CL().GetSwitchValueASCII(switches::kLockedPath).c_str());
  hex::SetDisabledNodeModules(
      CL().GetSwitchValueASCII(switches::kDisabledNodeModules).c_str());
  hex::SetEnabledNodeModules(
      CL().GetSwitchValueASCII(switches::kEnabledNodeModules).c_str());
  hex::SetDisableNodeThirdPartyNativeModules(
      CL().HasSwitch(switches::kDisableNodeThirdPartyNativeModules));
  hex::SetDisableFormApis(CL().HasSwitch(switches::kDisableFormApis));
  hex::SetLaunchNodeInAllPages(CL().HasSwitch(switches::kLaunchNodeInAllPages));
#endif  // HEXCLIENT
  node::Prepare();
}

}  // namespace hex
