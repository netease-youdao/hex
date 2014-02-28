# Copyright (c) 2013 NetEase Youdao Inc. and other heX contributors.
# Portions Copyright (c) 2008-2013 Marshall A.Greenblatt. Use of this
# source code is governed by a BSD-style license that can be found in the
# LICENSE file.

{
  'variables': {
    'pkg-config': 'pkg-config',
    'chromium_code': 1,
    'hexclient%': '<!(echo %HEXCLIENT%)',
    'no_devtools%': '<!(echo %NO_DEVTOOLS%)',
    #'repack_locales_cmd': ['python', '../cef/tools/repack_locales.py'],
    'grit_out_dir': '<(SHARED_INTERMEDIATE_DIR)/hex',
    'about_credits_file': '<(SHARED_INTERMEDIATE_DIR)/about_credits.html',
    'werror': '',
  },
  'includes': [
    # Bring in the source file lists.
    'hex_client_paths.gypi',
    '../chrome/version.gypi',
  ],

  'targets': [
    {
      'target_name': 'hex',
      'type': 'shared_library',
      'toolsets': ['host', 'target'],
      'include_dirs': [
        '.',
        '..',
        'client',
        '../cef',
        'src',
        '../v8/include',
        #'../third_party/node/src',
        #'../third_party/node/deps/uv/include',
      ],
      'dependencies': [
        '<(DEPTH)/base/base.gyp:base',
        '<(DEPTH)/v8/tools/gyp/v8.gyp:v8',
        #'<(DEPTH)/third_party/node/node.gyp:node',
      ],
      'defines': [
        'BUILDING_HEX_SHARED',
        'HEX_IMPLEMENTATION',
      ],
      'sources': [
        'src/hex_export.h',
        'src/hex_object.cc',
        'src/hex_object.h',
        'src/hex_shared.h',
        'src/hex_switches.cc',
        'src/hex_switches.h',
        'src/hex_switches_port.cc',
        'src/hex_switches_port.h',
        'src/default_values.h',
        'src/hex_use_node.h',
      ],
      'conditions': [
        [ 'OS=="win"', {
          'sources': [
            'src/hex_object_win.cpp',
            'src/hex_shared_win.h',
            'src/hex_shared_win.cpp',
            'src/hex_use_node_win.cc',
          ],
          'msvs_settings': {
            'VCLinkerTool': {
              'AdditionalDependencies': [ 'dwmapi.lib', ],
            },
          },
        }],
        [ 'OS=="mac"', {
          'sources': [
            'src/hex_use_node_mac.cc',
            'src/hex_shared_mac.cc',
          ],
          #'copies': [{
          #  'destination': '<(PRODUCT_DIR)/$(CONTENTS_FOLDER_PATH)/Libraries',
          #  'files': [
          #    '<(PRODUCT_DIR)/libhex.dylib',
          #    '<(PRODUCT_DIR)/libv8.dylib',
          #    '<(PRODUCT_DIR)/libnode.dylib',
          #    '<(PRODUCT_DIR)/libuv.dylib',
          #  ],
          #}],
        }],
        ['hexclient==1', {
          'defines': [
            'HEXCLIENT',
          ],
        }],
      ],
      'msvs_disabled_warnings': [
        4251,
        4996,
        4390,
        4800,
      ],
      'xcode_settings': {
        #'LD_DYLIB_INSTALL_NAME': '$(DYLIB_INSTALL_NAME_BASE:standardizepath)/Chromium Framework.framework/Libraries/libhex.dylib',
        #'DYLIB_INSTALL_NAME_BASE': '@executable_path/../Versions/<(version_full)',
        #'DYLIB_COMPATIBILITY_VERSION': '<(version_mac_dylib)',
        #'DYLIB_CURRENT_VERSION': '<(version_mac_dylib)',
        #'OTHER_LDFLAGS': [ '-Wl', '-ObjC' ],
        'DYLIB_INSTALL_NAME_BASE': '@rpath',
        'LD_RUNPATH_SEARCH_PATHS': [
          #'@loader_path/.',
          #'@loader_path/../Libraries/.',
          '@loader_path/../../../Chromium Framework.framework/Libraries/.',
          '@loader_path/../Versions/<(version_full)/Chromium Framework.framework/Libraries/.'
        ],
        'OTHER_LDFLAGS': ['-dynamiclib', '-all_load'],
      },
    },
  ],
  'conditions': [
    [ 'hexclient==1', {
      'variables': {
        'revision': '<!(python ../hex/tools/revision.py)',
        # Need to be creative to match dylib version formatting requirements.
        'version_mac_dylib':
        '<!(python ../chrome/tools/build/version.py -f VERSION -f ../chrome/VERSION -t "@HEX_MAJOR@.@HEX_MINOR@.@HEX_BUILD@_@BUILD_HI@.@BUILD_LO@" -e "BUILD_HI=int(BUILD)/256" -e "BUILD_LO=int(BUILD)%256")',
      },
      'targets': [
        {
          # Create the pack file for heX resources.
          'target_name': 'hex_resources',
          'type': 'none',
          'dependencies': [
            '<(DEPTH)/cef/cef.gyp:about_credits',
          ],
          'actions': [
            {
              'action_name': 'hex_resources',
              'variables': {
                'grit_grd_file': 'src/resources/hex_resources.grd',
                'grit_additional_defines': [
                  '-E', 'about_credits_file=<(about_credits_file)',
                ],
              },
              'includes': [ '../build/grit_action.gypi' ],
            },
          ],
          'conditions': [
            ['hexclient==1', {
              'defines': [
                'HEXCLIENT',
              ],
            }],
          ],
          'includes': [ '../build/grit_target.gypi' ],
          'copies': [
            {
              'destination': '<(PRODUCT_DIR)',
              'files': [
                '<(grit_out_dir)/hex_resources.pak'
              ],
            },
          ],
        },
        {
          'target_name': 'hexclient',
          'type': 'executable',
          'mac_bundle': 1,
          'msvs_guid': '70ABC794-F476-40D4-93DE-E10CEEB23B31',
          'dependencies': [
            '<(DEPTH)/cef/cef.gyp:cef_pak',
            '<(DEPTH)/cef/cef.gyp:libcef',
            '<(DEPTH)/cef/cef.gyp:libcef_dll_wrapper',
            '<(DEPTH)/third_party/jsoncpp/jsoncpp.gyp:jsoncpp',
            'hex',
          ],
          'defines': [
            'USING_CEF_SHARED',
            'CEF3',
          ],
          'include_dirs': [
            '.',
            '..',
            'client',
            'common',
            '../cef',
            'src',
            '../third_party/jsoncpp/source/include',
          ],
          'sources': [
            '<@(includes_common)',
            '<@(hex_includes_common)',
            '<@(includes_wrapper)',
            '<@(hexclient_sources_common)',
            '<@(hexclient_sources_common_cef3)',
          ],
          'mac_bundle_resources': [
            '<@(hexclient_bundle_resources_mac)',
          ],
          'mac_bundle_resources!': [
            # TODO(mark): Come up with a fancier way to do this (mac_info_plist?)
            # that automatically sets the correct INFOPLIST_FILE setting and adds
            # the file to a source group.
            'client/mac/Info.plist',
          ],
          'xcode_settings': {
            'INFOPLIST_FILE': 'client/mac/Info.plist',
            # Necessary to avoid an "install_name_tool: changing install names or
            # rpaths can't be redone" error.
            'OTHER_LDFLAGS': ['-Wl,-headerpad_max_install_names'],
          },
          'copies': [
            {
              'destination': '<(PRODUCT_DIR)',
              'files': [
                'client/res/manifest.json',
              ],
            }
          ],
          'conditions': [
            # For heX
            ['hexclient==1', {
              #'defines': [
              #  'USE_NODE',
              #],
              #'copies': [
              #  {
              #    'destination': '<(PRODUCT_DIR)',
              #    'files': [
              #      '<(DEPTH)/third_party/node/src/node.js',
              #    ],
              #  }
              #],
              'actions': [
                {
                  'action_name': 'nodejs',
                  'inputs': [
                    '<(DEPTH)/third_party/node/src/node.js',
                  ],
                  'outputs': [
                    '<(PRODUCT_DIR)/node.js'
                  ],
                  'action': [
                    'python',
                    'tools/launch_node_manually.py',
                    '<@(_outputs)',
                    '<@(_inputs)',
                  ],
                },
              ],
            }],
            ['no_devtools==1', {
              'defines': [
                'NO_DEVTOOLS',
              ],
            }],
            ['OS=="win" and win_use_allocator_shim==1', {
              'dependencies': [
                '<(DEPTH)/base/allocator/allocator.gyp:allocator',
              ],
            }],
            ['OS=="win"', {
              'configurations': {
                'Debug_Base': {
                  'msvs_settings': {
                    'VCLinkerTool': {
                      'LinkIncremental': '<(msvs_large_module_debug_link_mode)',
                    },
                  },
                },
              },
              'msvs_settings': {
                'VCLinkerTool': {
                  # Set /SUBSYSTEM:WINDOWS.
                  'SubSystem': '2',
                  'EntryPointSymbol' : 'wWinMainCRTStartup',
                  'AdditionalDependencies': [ 'dwmapi.lib', ],
                },
                'VCManifestTool': {
                  'AdditionalManifestFiles': [
                    'client/hexclient.exe.manifest',
                  ],
                },
              },
              'link_settings': {
                'libraries': [
                  '-lcomctl32.lib',
                  '-lshlwapi.lib',
                  '-lrpcrt4.lib',
                  '-lopengl32.lib',
                  '-lglu32.lib', 
                ],
              },
              'sources': [
                '<@(includes_win)',
                '<@(hexclient_sources_win)',
              ],
            }],
            ['OS == "win" or (toolkit_uses_gtk == 1 and selinux == 0)', {
              'dependencies': [
                '<(DEPTH)/sandbox/sandbox.gyp:sandbox',
              ],
            }],
            ['toolkit_uses_gtk == 1', {
              'dependencies': [
                '<(DEPTH)/build/linux/system.gyp:gtk',
              ],
            }],
            [ 'OS=="mac"', {
              'product_name': 'hexclient',
              'dependencies': [
                'hex_helper_app',
                'interpose_dependency_shim',
              ],
              'copies': [
                {
                  # Add library dependencies to the bundle.
                  'destination': '<(PRODUCT_DIR)/hexclient.app/Contents/Frameworks/Chromium Embedded Framework.framework/Libraries/',
                  'files': [
                    '<(PRODUCT_DIR)/libcef.dylib',
                    '<(PRODUCT_DIR)/ffmpegsumo.so',
                  ],
                },
                {
                  # Add localized resources to the bundle.
                  'destination': '<(PRODUCT_DIR)/hexclient.app/Contents/Frameworks/Chromium Embedded Framework.framework/Resources/',
                  'files': [
                    '<!@pymod_do_main(repack_locales -o -g <(grit_out_dir) -s <(SHARED_INTERMEDIATE_DIR) -x <(INTERMEDIATE_DIR) <(locales))',
                  ],
                },
                {
                  # Add the helper app.
                  'destination': '<(PRODUCT_DIR)/hexclient.app/Contents/Frameworks',
                  'files': [
                    '<(PRODUCT_DIR)/hexclient Helper.app',
                    '<(PRODUCT_DIR)/libplugin_carbon_interpose.dylib',
                  ],
                },
              ],
              'postbuilds': [
                {
                  'postbuild_name': 'Fix Framework Link',
                  'action': [
                    'install_name_tool',
                    '-change',
                    '@executable_path/libcef.dylib',
                    '@executable_path/../Frameworks/Chromium Embedded Framework.framework/Libraries/libcef.dylib',
                    '${BUILT_PRODUCTS_DIR}/${EXECUTABLE_PATH}'
                  ],
                },
                {
                  'postbuild_name': 'Copy WebCore Resources',
                  'action': [
                    'cp',
                    '-Rf',
                    '${BUILT_PRODUCTS_DIR}/../../third_party/WebKit/Source/core/Resources/',
                    '${BUILT_PRODUCTS_DIR}/${PRODUCT_NAME}.app/Contents/Frameworks/Chromium Embedded Framework.framework/Resources/'
                  ],
                },
                {
                  'postbuild_name': 'Copy locale Resources',
                  'action': [
                    'cp',
                    '-Rf',
                    '${BUILT_PRODUCTS_DIR}/locales/',
                    '${BUILT_PRODUCTS_DIR}/${PRODUCT_NAME}.app/Contents/Frameworks/Chromium Embedded Framework.framework/Resources/'
                  ],
                },
                {
                  'postbuild_name': 'Copy cef.pak File',
                  'action': [
                    'cp',
                    '-f',
                    '${BUILT_PRODUCTS_DIR}/cef.pak',
                    '${BUILT_PRODUCTS_DIR}/hexclient.app/Contents/Frameworks/Chromium Embedded Framework.framework/Resources/cef.pak'
                  ],
                },
                {
                  'postbuild_name': 'Copy devtools_resources.pak File',
                  'action': [
                    'cp',
                    '-f',
                    '${BUILT_PRODUCTS_DIR}/devtools_resources.pak',
                    '${BUILT_PRODUCTS_DIR}/hexclient.app/Contents/Frameworks/Chromium Embedded Framework.framework/Resources/devtools_resources.pak'
                  ],
                },
                {
                  # Modify the Info.plist as needed.
                  'postbuild_name': 'Tweak Info.plist',
                  'action': ['../build/mac/tweak_info_plist.py',
                             '--scm=1'],
                },
                {
                  # This postbuid step is responsible for creating the following
                  # helpers:
                  #
                  # hexclient Helper EH.app and hexclient Helper NP.app are created
                  # from hexclient Helper.app.
                  #
                  # The EH helper is marked for an executable heap. The NP helper
                  # is marked for no PIE (ASLR).
                  'postbuild_name': 'Make More Helpers',
                  'action': [
                    '../build/mac/make_more_helpers.sh',
                    'Frameworks',
                    'hexclient',
                  ],
                },
              ],
              'link_settings': {
                'libraries': [
                  '$(SDKROOT)/System/Library/Frameworks/AppKit.framework',
                ],
              },
              'sources': [
                '<@(includes_mac)',
                '<@(hexclient_sources_mac)',
              ],
            }],
            [ 'OS=="linux" or OS=="freebsd" or OS=="openbsd"', {
              'sources': [
                '<@(includes_linux)',
                '<@(hexclient_sources_linux)',
              ],
              'copies': [
                {
                  'destination': '<(PRODUCT_DIR)/files',
                  'files': [
                    '<@(hexclient_bundle_resources_linux)',
                  ],
                },
              ],
            }],
          ],
        },
      ],
    }],
    [ 'os_posix==1 and OS!="mac" and OS!="android" and gcc_version>=46', {
      'target_defaults': {
        # Disable warnings about c++0x compatibility, as some names (such
        # as nullptr) conflict with upcoming c++0x types.
        'cflags_cc': ['-Wno-c++0x-compat'],
      },
    }],
  ],
}
