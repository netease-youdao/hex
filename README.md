![heX logo](http://netease-youdao.github.io/hex/hex_full_logo.png)

heX
================================================================================
#### A Solution for Building Desktop Application Using HTML5 and Node.js

heX is a solution that provides a new way to build cross-platform desktop application using the web technology. heX bases on CEF and merges Chromium and Node.js, so that the modules or third-party extensions of Node.js can be "required" in the web page, and the DOM element of HTML can be visited in Node.js' modules. heX can also be embedded as a web container module into the desktop projects.

 * Homepage: <http://hex.youdao.com>
 * Mailing list: <https://groups.google.com/group/youdao_hex>
 * Documentation: <http://netease-youdao.github.io/hex/>
 * Wiki: <https://github.com/netease-youdao/hex/wiki>
 * Issues: <https://github.com/netease-youdao/hex/issues>


## Features

### Efficient
 * Chromium's new web features
 * Multiple/Single process architecture
 * Web developer tools

### Easy
 * Using JavaScript, CSS, HTML
 * Low cost of learning, the main API from Chromium and Node.js
 * Simply by configuring manifest to use

### Extensible
 * Plenty of third-party JavaScript and C++ extension of Node.js can be used


## Why should I use heX?

HTML5 standard was designated as a Candidate Recommandation in December 2012. Chromium as a rapid development of open source project has good support for HTML5 for a long time. And Node.js using an event-driven, high throughput non-blocking I/O model provides a variety of native operating API, so that we can do more via JavaScript. heX merges them and realize complementary advantages, thus we can use it to develop desktop application via pure web front-end technology. 

heX has the following advantages:
 * Easy to use, web developer can use their best technology(browser and Node.js APIs), the cost of learning is quite low.
 * Desiging GUI is as efficient as we debug a web page in the Chrome.
 * Based on Node.js convenient extension mechanism, the function is unlimited.


## How to develop with heX?

Start to develop a desktop application with heX is quite simple. With the following references in Wiki, you can get started quickly. [ links are coming soon. ]

 * Quick Start Guide
 * heX Manifest Instructions
 * heX Native API Instructions
 * FAQ

We can write the code like this:
```js
var fs = require('fs');
var path = require('path');
var directory = $('#directory');
var fileList = $('#filelist');
directory.value = process.cwd();
if (!fs.existsSync(dir)) {
  ...
} else {
  fs.readdir(dir, function(err, list) {
    ...
  }
}
```


## Binary Distributions

There are 6 kinds of binary distributions.

 * [heX end user distribution] (http://hex.youdao.com/downloads/hex_1453_end_user_windows.zip)
 * [heX end user distribution without Node.js] (http://hex.youdao.com/downloads/hex_1453_end_user_no_node_windows.zip)
 * [heX web develop distribution] (http://hex.youdao.com/downloads/hex_1453_web_develop_windows.zip)
 * [heX web develop distribution without Node.js] (http://hex.youdao.com/downloads/hex_1453_web_develop_no_node_windows.zip)
 * [heX module distribution] (http://hex.youdao.com/downloads/hex_module_1453_windows.zip)
  * [heX module PDB] (http://hex.youdao.com/downloads/hex_module_1453_windows_symbols.zip)
 * [heX module distribution without Node.js] (http://hex.youdao.com/downloads/hex_module_1453_no_node_windows.zip)
  * [heX module PDB without Node.js] (http://hex.youdao.com/downloads/hex_module_1453_no_node_windows_symbols.zip)


## Authors


## License

New BSD License

Copyright (c) 2012-2013 NetEase Youdao Inc. and other heX contributors.
Portions Copyright (c) 2008-2013 Marshall A.Greenblatt, 2006-2009
Google Inc., and Joyent, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
  * Neither the name of Google Inc. nor the name Chromium Embedded Framework
    nor the name of Joyent, Inc. nor the name of NetEase Youdao Inc. nor
    the name heX nor the names of its contributors may be used to endorse
    or promote products derived from this software without specific prior
    written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
