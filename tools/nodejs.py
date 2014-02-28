# Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors. All
# rights reserved. Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file.

import pickle
from optparse import OptionParser
import os
import sys
import platform


# cannot be loaded as a module
if __name__ != "__main__":
    sys.stderr.write('This file cannot be loaded as a module!')
    sys.exit()
    

# parse command-line options
disc = """
This utility download and configure Node.JS.
"""

parser = OptionParser(description=disc)
parser.add_option('--nodejs-rev', dest='nodejsrev', help='Node.JS revision')
(options, args) = parser.parse_args()

# the option is required
if options.nodejsrev is None:
    parser.print_help(sys.stdout)
    sys.exit()

curr_dir = os.getcwd()
os.chdir('../third_party')

if os.path.exists('node'):
    sys.exit()

os.system('gclient config git://github.com/joyent/node.git --name node ' +
          '--gclientfile node.gclient')
os.system('gclient sync --revision node@' + options.nodejsrev +
          ' --force --reset --nohooks --gclientfile node.gclient')
system = platform.system().lower()
if system == 'windows':
    os.system(os.path.abspath('node/vcbuild.bat noetw noperfctr'))
elif system == 'darwin' or system == 'linux':
    os.system(os.path.abspath('node/configure noetw noperfctr xcode'))
    
os.chdir(curr_dir)
