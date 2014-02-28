# Copyright (c) 2011 The Chromium Embedded Framework Authors.
# Portions copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

from gclient_util import *
import os, sys
from optparse import OptionParser

parser = OptionParser(description="")
parser.add_option('--hexclient', dest='hexclient', help='This is a hexclient.')
parser.add_option('--no-devtools', dest='nodevtools', action='store_true',
                  default=False, help='Disable devtools (End-user distrib).')
(options, args) = parser.parse_args()

hex_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir))

print "\nCheckout CEF..."
cef = [ 'python', 'tools/cef.py', 
          '--cef-rev', '1450'];
RunAction(hex_dir, cef)

cef_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir,
                                       os.pardir, 'cef'))

print "\nChecking CEF and Chromium revisions..."
gyper = [ 'python', 'tools/check_revision.py' ]
RunAction(cef_dir, gyper)

print "\nGenerating CEF version header file..."
gyper = [ 'python', 'tools/make_version_header.py',
          '--header', 'include/cef_version.h',
          '--cef_version', 'VERSION',
          '--chrome_version', '../chrome/VERSION',
          '--cpp_header_dir', 'include' ]
RunAction(cef_dir, gyper)

print "\nCloning and configuring Node.JS..."
nodejs = [ 'python', 'tools/nodejs.py', 
           '--nodejs-rev', '04018d4b3938fd30ba14822e79195e4af2be36f6']
RunAction(hex_dir, nodejs)

print "\nPatching build configuration and source files for heX..."
patcher = [ 'python', 'tools/patcher.py', 
            '--patch-config', 'patch/patch.cfg' ]
os.environ['CEF3'] = 'CEF3'
RunAction(hex_dir, patcher)

print "\nPatching build configuration and source files for CEF..."
patcher = [ 'python', 'tools/patcher.py', 
            '--patch-config', 'patch/patch.cfg' ]
RunAction(cef_dir, patcher)

print "\nGenerating heX version header file..."
gyper = [ 'python', 'tools/make_version_header.py',
          '--header', 'include/hex_version.h',
          '--hex_version', 'VERSION',
          '--chrome_version', '../chrome/VERSION',
          '--cpp_header_dir', 'include' ]
RunAction(hex_dir, gyper)

print "\nGenerating heX project files..."
os.environ['HEX_DIRECTORY'] = os.path.basename(hex_dir);
os.environ['HEXCLIENT'] = options.hexclient;
os.environ['BUILD_POSTFIX'] = 'build';
if options.nodevtools:
    os.environ['NO_DEVTOOLS'] = '1'
gyper = [ 'python', 'tools/gyp_hex', 'hex.gyp', '-I', 'hex.gypi' ]
RunAction(hex_dir, gyper)
