# Copyright (c) 2013 NetEase Youdao Inc. and other heX contributors.
# Portions copyright (c) 2013 The Chromium Embedded Framework Authors.
# All rights reserved. Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

from date_util import *
from file_util import *
from optparse import OptionParser
import svn_util as svn
import git_util as git
import sys

# cannot be loaded as a module
if __name__ != "__main__":
    sys.stderr.write('This file cannot be loaded as a module!')
    sys.exit()


# parse command-line options
disc = """
This utility creates the version header file.
"""

parser = OptionParser(description=disc)
parser.add_option('--header', dest='header', metavar='FILE',
                  help='output version header file [required]')
parser.add_option('--hex_version', dest='hex_version', metavar='FILE',
                  help='input heX version config file [required]')
parser.add_option('--chrome_version', dest='chrome_version', metavar='FILE',
                  help='input Chrome version config file [required]')
parser.add_option('--cpp_header_dir', dest='cpp_header_dir', metavar='DIR',
                  help='input directory for C++ header files [required]')
parser.add_option('-q', '--quiet',
                  action='store_true', dest='quiet', default=False,
                  help='do not output detailed status information')
(options, args) = parser.parse_args()

# the header option is required
if options.header is None or options.hex_version is None or options.chrome_version is None or options.cpp_header_dir is None:
    parser.print_help(sys.stdout)
    sys.exit()

def write_svn_header(header, chrome_version, hex_version, cpp_header_dir):
    """ Creates the header file for the current revision and Chrome version information
       if the information has changed or if the file doesn't already exist. """

    if not path_exists(chrome_version):
      raise Exception('Chrome version file '+chrome_version+' does not exist.')
    if not path_exists(hex_version):
      raise Exception('heX version file '+hex_version+' does not exist.')

    args = {}
    read_version_file(chrome_version, args)
    read_version_file(hex_version, args)

    if path_exists(header):
        oldcontents = read_file(header)
    else:
        oldcontents = ''

    year = get_year()

    if os.path.exists(os.path.join('.', '.svn')):
      revision = svn.get_revision()
    elif os.path.exists(os.path.join('.', '.git')):
      revision = git.get_svn_revision()
    else:
      revision = '"unknown"'

    newcontents = '// Copyright (c) '+year+' NetEase Youdao Inc. and other heX contributors.\n'+\
                  '// Portions Copyright (c) 2008-2013 Marshall A.Greenblatt, 2006-2009\n'+\
                  '// Google Inc., and Joyent, Inc. All rights reserved.\n'+\
                  '//\n'+\
                  '// Redistribution and use in source and binary forms, with or without\n'+\
                  '// modification, are permitted provided that the following conditions are\n'+\
                  '// met:\n'+\
                  '//\n'+\
                  '//    * Redistributions of source code must retain the above copyright notice,\n'+\
                  '// this list of conditions and the following disclaimer.\n'+\
                  '//    * Redistributions in binary form must reproduce the above copyright\n'+\
                  '// notice, this list of conditions and the following disclaimer in the\n'+\
                  '// documentation and/or other materials provided with the distribution.\n'+\
                  '//    * Neither the name of Google Inc. nor the name Chromium Embedded Framework\n'+\
                  '// nor the name of Joyent, Inc. nor the name of NetEase Youdao Inc. nor\n'+\
                  '// the name heX nor the names of its contributors may be used to endorse\n'+\
                  '// or promote products derived from this software without specific prior\n'+\
                  '// written permission.\n'+\
                  '//\n'+\
                  '// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS\n'+\
                  '// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT\n'+\
                  '// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR\n'+\
                  '// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT\n'+\
                  '// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,\n'+\
                  '// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT\n'+\
                  '// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,\n'+\
                  '// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY\n'+\
                  '// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT\n'+\
                  '// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE\n'+\
                  '// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n'+\
                  '//\n'+\
                  '// ---------------------------------------------------------------------------\n'+\
                  '//\n'+\
                  '// This file is generated by the make_version_header.py tool.\n'+\
                  '//\n\n'+\
                  '#ifndef HEX_INCLUDE_HEX_VERSION_H_\n'+\
                  '#define HEX_INCLUDE_HEX_VERSION_H_\n\n'+\
                  '#define HEX_VERSION_MAJOR ' + args['HEX_MAJOR'] + '\n'+\
                  '#define HEX_VERSION_MINOR ' + args['HEX_MINOR'] + '\n'+\
                  '#define HEX_VERSION_BUILD ' + args['HEX_BUILD'] + '\n'+\
                  '#define HEX_REVISION ' + revision[0:9] + '\n'+\
                  '#endif  // HEX_INCLUDE_HEX_VERSION_H_\n'
    if newcontents != oldcontents:
        write_file(header, newcontents)
        return True

    return False

written = write_svn_header(options.header, options.chrome_version, options.hex_version, options.cpp_header_dir)
if not options.quiet:
  if written:
    sys.stdout.write('File '+options.header+' updated.\n')
  else:
    sys.stdout.write('File '+options.header+' is already up to date.\n')
