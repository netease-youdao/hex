# Copyright (c) 2012 Youdao. All rights reserved. Use of this source code is
# governed by a BSD-style license that can be found in the LICENSE file.

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
parser.add_option('--cef-rev', dest='cefrev', help='CEF3 revision')
(options, args) = parser.parse_args()

# the option is required
if options.cefrev is None:
    parser.print_help(sys.stdout)
    sys.exit()

curr_dir = os.getcwd()
os.chdir('..')

if os.path.exists('cef'):
    sys.exit()

os.system('gclient config http://chromiumembedded.googlecode.com/svn/trunk/cef3' +
          ' --name cef --gclientfile cef.gclient')
os.system('gclient sync --revision cef@' + options.cefrev +
          ' --force --reset --nohooks --gclientfile cef.gclient')
    
os.chdir(curr_dir)
