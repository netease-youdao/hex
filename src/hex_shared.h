// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
// rights reserved. Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

#ifndef HEX_SRC_HEX_SHARED_H
#define HEX_SRC_HEX_SHARED_H

#include "hex_export.h"
#include "hex_switches_port.h"
#include <string>
#include <map>

namespace hex {

enum FormStyleType {
  STANDARD = 0,
  CAPTIONLESS,
  DESKTOP_WIDGET
};

enum SystemCommand {
  RESTORE = 1000,
  MOVE,
  SIZE,
  MINIMIZE,
  MAXIMIZE,
  CLOSE
};

enum FormState {
  NORMAL = 0,
  MINIMIZED,
  MAXIMIZED,
  HIDDEN,
  FULLSCREEN
};

enum FormActivation {
  ACTIVATED = 0,
  INACTIVATED
};

enum FormPosition {
  SCREENCENTERED = -3,
  PARENTCENTERED,
  SYSTEMDEFAULT
};

enum FormBorderDirection {
  TOP = 0,
  TOPRIGHT,
  RIGHT,
  BOTTOMRIGHT,
  BOTTOM,
  BOTTOMLEFT,
  LEFT,
  TOPLEFT
};

enum Other {
  FULL = -2,
  NONE = -1
};

HEX_EXPORT int GetMainProcessID();
HEX_EXPORT std::string GetApplicationDirectoryPath();
HEX_EXPORT std::wstring GetApplicationDirectoryPathW();

}  // namespace hex

#endif  // HEX_SRC_HEX_SHARED_H