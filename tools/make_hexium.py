# Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
# rights reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

from date_util import *
from file_util import *
from gclient_util import *
from optparse import OptionParser
import os
import re
import shlex
import subprocess
from svn_util import *
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

def create_7z_archive(input_dir, zip_file):
  """ Creates a 7z archive of the specified input directory. """
  command = os.environ['CEF_COMMAND_7ZIP']
  run('"' + command + '" a -y ' + zip_file + ' ' + input_dir, os.path.split(zip_file)[0])

def create_output_dir(name, parent_dir):
  """ Creates an output directory and adds the path to the archive list. """
  output_dir = os.path.abspath(os.path.join(parent_dir, name))
  remove_dir(output_dir, options.quiet)
  make_dir(output_dir, options.quiet)
  archive_dirs.append(output_dir)
  return output_dir

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
This utility builds the HeXium Binary Distribution.
"""

parser = OptionParser(description=disc)
parser.add_option('--output-dir', dest='outputdir', metavar='DIR',
                  help='output directory [required]')
parser.add_option('--no-archive',
                  action='store_true', dest='noarchive', default=False,
                  help='don\'t create archives for output directories')
parser.add_option('-q', '--quiet',
                  action='store_true', dest='quiet', default=False,
                  help='do not output detailed status information')
(options, args) = parser.parse_args()

# Test the operating system.
platform = '';
if sys.platform == 'win32':
  platform = 'windows'
elif sys.platform == 'darwin':
  platform = 'macosx'
elif sys.platform.startswith('linux'):
  platform = 'linux'

# the outputdir option is required
if options.outputdir is None:
  parser.print_help(sys.stderr)
  sys.exit()

# script directory
script_dir = os.path.dirname(__file__)

# heX root directory
hex_dir = os.path.abspath(os.curdir)

# src directory
src_dir = os.path.abspath(os.path.join(hex_dir, os.pardir))

# build directory
if platform == 'windows':
  build_dir = os.path.abspath(os.path.join(src_dir, 'build', 'Release'))
elif platform == 'macosx':
  build_dir = os.path.abspath(os.path.join(src_dir, 'xcodebuild', 'Release'))


# retrieve url, revision and date information
chromium_info = get_svn_info(os.path.join(hex_dir, os.pardir))
chromium_url = chromium_info['url']
chromium_rev = chromium_info['revision']
date = get_date()

# Read and parse the version file (key=value pairs, one per line)
args = {}
read_version_file(os.path.join(hex_dir, '../chrome/VERSION'), args)
read_version_file(os.path.join(hex_dir, 'VERSION'), args)

hex_url = 'http://hex.youdao.com'
hex_ver = args['HEX_MAJOR']+'.'+args['HEX_MINOR']+'.'+args['HEX_BUILD']+'_'+args['BUILD']

chromium_ver = args['MAJOR']+'.'+args['MINOR']+'.'+args['BUILD']+'.'+args['PATCH']

# list of output directories to be archived
archive_dirs = []

platform_arch = '32'

# output directory
output_dir_base = 'hexium_' + hex_ver
output_dir_name = output_dir_base + '_' + platform + platform_arch

output_dir = create_output_dir(output_dir_name, options.outputdir)

# transfer the LICENSE.txt file
copy_file(os.path.join(hex_dir, 'LICENSE'), output_dir, options.quiet)
copy_file(os.path.join(hex_dir, 'AUTHORS'), output_dir, options.quiet)
copy_file(os.path.join(hex_dir, 'README.md'), output_dir, options.quiet)

if platform == 'windows':
  copy_file(os.path.join(build_dir, 'chrome.dll'), output_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'chrome.pak'), output_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'chrome_100_percent.pak'), output_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'chrome_child.dll'), output_dir, options.quiet)
  copy_files(os.path.join(build_dir, 'd3dcompiler_*.dll'), output_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'ffmpegsumo.dll'), output_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'hex.dll'), output_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'hexium.exe'), output_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'icudt.dll'), output_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'libEGL.dll'), output_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'libGLESv2.dll'), output_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'metro_driver.dll'), output_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'node.dll'), output_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'resources.pak'), output_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'uv.dll'), output_dir, options.quiet)
  copy_file(os.path.join(build_dir, 'v8.dll'), output_dir, options.quiet)
  copy_dir(os.path.join(build_dir, 'locales'), os.path.join(output_dir, 'locales'), options.quiet)

elif platform == 'macosx':
  copy_dir(os.path.join(build_dir, 'Chromium.app'), os.path.join(output_dir, 'Chromium.app'), options.quiet)

if not options.noarchive:
  # create an archive for each output directory
  archive_extenstion = '.zip'
  if os.getenv('CEF_COMMAND_7ZIP', '') != '':
    archive_extenstion = '.7z'
  for dir in archive_dirs:
    zip_file = os.path.split(dir)[1] + archive_extenstion
    if not options.quiet:
      sys.stdout.write('Creating '+zip_file+"...\n")
    if archive_extenstion == '.zip':
      create_archive(dir, os.path.join(dir, os.pardir, zip_file))
    else:
      create_7z_archive(dir, os.path.join(dir, os.pardir, zip_file))
