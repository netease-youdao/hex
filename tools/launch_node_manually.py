#!/usr/bin/env python
#

# Copyright (c) 2012 Youdao. All rights reserved. Use of this source code is
# governed by a BSD-style license that can be found in the LICENSE file.


import os
from os.path import dirname
import re
import sys


def main():
  output = sys.argv[1]
  input = sys.argv[2]
  sys.stdout.write(input)
  sys.stdout.write(output)
  input_file = open(input, 'r')
  try:
    text = input_file.read()
  finally:
    input_file.close()
  text = text[0:len(text)-2] + '.call(window, window.process);'
  output_file = open(output, 'w')
  try:
    output_file.write(text)
  finally:
    output_file.close()
  

if __name__ == "__main__":
  main()
