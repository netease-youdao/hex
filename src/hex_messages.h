// Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. 
// Portions copyright (c) 2011 The Chromium Embedded Framework Authors.
// Portions copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file.

// IPC messages for heX.

#include "base/memory/shared_memory.h"
#include "content/public/common/common_param_traits.h"
#include "content/public/common/referrer.h"
#include "ipc/ipc_message_macros.h"

// TODO(cef): Re-using the message start for extensions may be problematic in
// the future. It would be better if ipc_message_utils.h contained a value
// reserved for consumers of the content API.
// See: http://crbug.com/110911
#define IPC_MESSAGE_START ExtensionMsgStart

