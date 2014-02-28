# Copyright (c) 2013 NetEase Youdao Inc. and other heX contributors.
# Portions Copyright (c) 2008-2013 Marshall A.Greenblatt. Use of this
# source code is governed by a BSD-style license that can be found in the
# LICENSE file.

{
  'variables': {
    #'proprietary_codecs': 1,
    'conditions': [
      # Directory for CEF source files.
      [ 'OS=="win"', {
        'hex_directory' : '<!(echo %HEX_DIRECTORY%)',
        # Keep the build output in the heX directory.
        'build_dir_prefix': '..\\<!(echo %HEX_DIRECTORY%)\\<!(echo %BUILD_POSTFIX%)\\',
        # Use SKIA text rendering for transparency support.
        'enable_skia_text': 1,
        # Disable tcmalloc's debugallocation to avoid crashing during startup.
        'disable_debugallocation': 1,
      }, { # OS!="win"
        'hex_directory' : '<!(echo $HEX_DIRECTORY)',
      }],
      [ 'OS=="mac"', {
        # Don't use the chrome style plugin with CEF.
        'clang_use_chrome_plugins': 0,
      }],
    ]
  }, 'conditions': [
    ['os_posix==1 and OS!="mac" and OS!="android"', {
      'target_defaults': {
        'cflags_cc': ['-Wno-deprecated-declarations'],
      },
    }]
  ]
}
