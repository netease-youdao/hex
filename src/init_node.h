// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#ifndef HEX_SRC_INIT_NODE_H_
#define HEX_SRC_INIT_NODE_H_

#include <string>
#include <vector>

#include "third_party/node/deps/uv/include/uv.h"
#include "third_party/node/src/node.h"
#include "hex/src/hex_switches.h"
#include "hex/src/hex_switches_port.h"
#include "base/message_loop/message_loop.h"
#include "base/command_line.h"
#include "content/public/common/content_switches.h"

namespace hex {

void ProcessNodeArguments(std::vector<std::string> args);
void InitializeNode(std::vector<std::string> args);

#define INIT_NODE() { std::vector<std::string> args; \
  args.push_back("node"); \
  args.push_back(""); \
  args.push_back(hex::NodeArgs()); \
  hex::InitializeNode(args); \
}

}  // namespace hex

#endif  // HEX_SRC_INIT_NODE_H_
