# Copyright (c) 2011 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

from date_util import *
from file_util import *
from gclient_util import *
from optparse import OptionParser
import os
import re
import shlex
import subprocess
import svn_util as svn
import git_util as git
import sys
import zipfile

def create_archive(input_dir, zip_file):
  """ Creates a zip archive of the specified input directory. """
  zf = zipfile.ZipFile(zip_file, 'w', zipfile.ZIP_DEFLATED)
  def addDir(dir):
    for f in os.listdir(dir):
      full_path = os.path.join(dir, f)
      if os.path.isdir(full_path):
        addDir(full_path)
      else:
        zf.write(full_path, os.path.relpath(full_path, \
                 os.path.join(input_dir, os.pardir)))
  addDir(input_dir)
  zf.close()

def create_readme(src, output_dir, hex_url, hex_rev, hex_ver, cef_url, cef_rev, \
                  cef_ver, chromium_url, chromium_rev, chromium_ver, \
                  date, usenode):
  """ Creates the README.TXT file. """
  data = read_file(src)
  data = data.replace('$HEX_URL$', hex_url)
  data = data.replace('$HEX_REV$', hex_rev)
  data = data.replace('$HEX_VER$', hex_ver)
  data = data.replace('$CEF_URL$', cef_url)
  data = data.replace('$CEF_REV$', cef_rev)
  data = data.replace('$CEF_VER$', cef_ver)
  data = data.replace('$CHROMIUM_URL$', chromium_url)
  data = data.replace('$CHROMIUM_REV$', chromium_rev)
  data = data.replace('$CHROMIUM_VER$', chromium_ver)
  data = data.replace('$DATE$', date)
  node = ''
  if usenode == 1:
    node = '\nnode.dll                Node.js and libuv core library.\nlibuv.dll\n'
  data = data.replace('$NODE$', node)
  write_file(os.path.join(output_dir, 'README.md'), data)
  if not options.quiet:
    sys.stdout.write('Creating README.md file.\n')

def eval_file(src):
  """ Loads and evaluates the contents of the specified file. """
  return eval(read_file(src), {'__builtins__': None}, None)

def run(command_line, working_dir):
  """ Run a command. """
  sys.stdout.write('-------- Running "'+command_line+'" in "'+\
                   working_dir+'"...'+"\n")
  args = shlex.split(command_line.replace('\\', '\\\\'))
  return subprocess.check_call(args, cwd=working_dir, env=os.environ,
                               shell=(sys.platform == 'win32'))

# cannot be loaded as a module
if __name__ != "__main__":
  sys.stderr.write('This file cannot be loaded as a module!')
  sys.exit()

# parse command-line options
disc = """
This utility builds the CEF Binary Distribution.
"""

parser = OptionParser(description=disc)
parser.add_option('--output-dir', dest='outputdir', metavar='DIR',
                  help='output directory [required]')
parser.add_option('--usenode', dest='usenode', help='Use Node.jsin heX. [required]')
parser.add_option('-q', '--quiet',
                  action='store_true', dest='quiet', default=False,
                  help='do not output detailed status information')
(options, args) = parser.parse_args()

os.environ['USE_NODE'] = options.usenode;
options.usenode = int(options.usenode)

# the outputdir option is required
if options.outputdir is None:
  parser.print_help(sys.stdout)
  sys.exit()

# script directory
script_dir = os.path.dirname(__file__)

# heX root directory
hex_dir = os.path.abspath(os.curdir)

# CEF root directory
cef_dir = os.path.abspath(os.path.join(script_dir, os.pardir, os.pardir, 'cef'))

# src directory
src_dir = os.path.abspath(os.path.join(cef_dir, os.pardir))

# retieve heX's url, revision and date information
if os.path.exists(os.path.join(hex_dir, '.svn')):
  hex_rev = svn.get_revision()
elif os.path.exists(os.path.join(hex_dir, '.git')):
  hex_rev = git.get_svn_revision()
else:
  hex_rev = '"unknown"'

hex_info = svn.get_svn_info(hex_dir)
hex_url = hex_info['url']

# retrieve url, revision and date information
cef_info = svn.get_svn_info(cef_dir)
cef_url = cef_info['url']
cef_rev = cef_info['revision']
chromium_info = svn.get_svn_info(os.path.join(cef_dir, os.pardir))
chromium_url = chromium_info['url']
chromium_rev = chromium_info['revision']
date = get_date()

# Read and parse the version file (key=value pairs, one per line)
args = {}
read_version_file(os.path.join(hex_dir, 'VERSION'), args)
read_version_file(os.path.join(cef_dir, 'VERSION'), args)
read_version_file(os.path.join(cef_dir, '../chrome/VERSION'), args)

hex_ver = args['HEX_MAJOR']+'.'+args['HEX_MINOR']+'.'+args['HEX_BUILD']+'_'+args['BUILD']
cef_ver = args['CEF_MAJOR']+'.'+args['BUILD']+'.'+cef_rev
chromium_ver = args['MAJOR']+'.'+args['MINOR']+'.'+args['BUILD']+'.'+args['PATCH']

# Test the operating system.
platform = '';
if sys.platform == 'win32':
  platform = 'windows'
elif sys.platform == 'darwin':
  platform = 'macosx'
elif sys.platform.startswith('linux'):
  platform = 'linux'

# output directory
output_dir = os.path.abspath(os.path.join(options.outputdir, \
                                          'hex_'+hex_ver+'_'+platform))
remove_dir(output_dir, options.quiet)
make_dir(output_dir, options.quiet)

# transfer the AUTHOR.txt and LICENSE.txt file
copy_file(os.path.join(hex_dir, 'AUTHORS'), output_dir, options.quiet)
copy_file(os.path.join(hex_dir, 'LICENSE'), output_dir, options.quiet)

build_dir_name = 'build'
if options.usenode != 1:
  build_dir_name = 'build_no_node'

if platform == 'windows':
  # create the README.TXT file
  hex_rev = '0'
  hex_url = 'http://www.hexlib.org'
  create_readme(os.path.join(script_dir, 'distrib/win/README.md'), output_dir, \
                hex_url, hex_rev, hex_ver, cef_url, cef_rev, cef_ver, \
                chromium_url, chromium_rev, chromium_ver, date, options.usenode)

  build_dir = os.path.join(hex_dir, build_dir_name, 'Release');
  dst_dir = output_dir
  copy_files(os.path.join(script_dir, '../../cef/tools/distrib/win/*.dll'), dst_dir, options.quiet)
  copy_files(os.path.join(build_dir, '*.dll'), dst_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'hexclient.exe'), dst_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'manifest.json'), dst_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'hex_resources.pak'), dst_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'fs.html'), dst_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'cp.html'), dst_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'http.html'), dst_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'cef.pak'), dst_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'devtools_resources.pak'), dst_dir, options.quiet)
  copy_dir(os.path.join(build_dir, 'locales'), os.path.join(dst_dir, 'locales'), \
           options.quiet)

# Create an archive of the output directory
zip_file = os.path.split(output_dir)[1] + '.zip'
if not options.quiet:
  sys.stdout.write('Creating '+zip_file+"...\n")
create_archive(output_dir, os.path.join(output_dir, os.pardir, zip_file))
