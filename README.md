![heX logo](http://hex.youdao.com/images/hex.png)

<!-- heX -->
================================================================================
### A Solution to Building Desktop Application Using HTML5 and Node.js

heX is a solution that provides a new way to build cross-platform desktop application using web technology. heX is based on CEF and integrates Chromium and Node.js. Modules or third-party extensions of Node.js can be "required" from the web page, while the DOM element of HTML can also be visited from Node.js' modules. In addition, heX can be embedded as a web container module into desktop projects, allowing desktop applications to take advantage of HTML5 features.

 * Homepage: <http://hex.youdao.com>
 * HeXium homepage: <http://hex.youdao.com/hexium>
 * Downloads: <http://hex.youdao.com/downloads>
 * Mailing list: <https://groups.google.com/group/youdao_hex>
 * Documentation: <http://hex.youdao.com/documentation>
 * Wiki: <https://github.com/netease-youdao/hex/wiki>
 * Issues: <https://github.com/netease-youdao/hex/issues>


## Features

### Powerful but Easy to Use
 * Chromium's new web features at your fingertips
 * Multiple/Single process architecture
 * Works with major web developer tools

### Low Learning Cost 
 * Supporting JavaScript, CSS, HTML
 * Main API consistent with those of Chromium and Node.js
 * Easy integration into your applications by simply configuring the manifest

### Extensible
 * Supporting any third-party JavaScript and C++ extension of Node.js
 * A platform to share the special heX extensions

### Debugging Node.js in developer tools
 * HeXium = Chromium + Node.js


## Why should I use heX?

HTML5 standard was designated as a Candidate Recommendation in December 2012. Chromium as a rapid development of open source project has good support for HTML5 for a long time. On the other hand, Node.js, which uses an event-driven, high throughput non-blocking I/O model, provides a variety of native operating APIs and enables a rich set of functionalities for JavaScript. heX integrates the two and retains advantages of both components. It enables one to develop desktop applications via pure web front-end technology.

heX has the following advantages:
 * Easy to use, web developer can use their favorite technology (browser and Node.js APIs), the learning cost is quite low.
 * Designing GUI is as efficient as we debug a web page in the Chrome.
 * Based on Node.js convenient extension mechanism, the functionalities can be easily extended.


## Why was HeXium born

A solution for developing and debugging traditional web front and Node.js even more JavaScript host environments by the cross-platform way.
  * Developer tools for heX as we develop web pages in Chrome
  * An environment to debug Node.js.


## How to develop with heX?

Developing a desktop application with heX is very simple. The following Wiki references allow you to get started quickly. [ links are coming soon. ]

 * Quick Start Guide
 * heX Manifest Instructions
 * heX Native API Instructions
 * FAQ


A piece of sample code looks like this:
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

There are 4 kinds of binary distributions each platform. Current version: v1.1.6_1612
 * Windows
   * heX end user distribution
   * [heX web develop distribution](http://netease-youdao.github.io/hex/downloads/hex_1.1.6_1621_windows32_client.zip)
   * [heX module distribution](http://netease-youdao.github.io/hex/downloads/hex_module_1.1.6_1621_windows32.zip)
     * [heX module debug PDB](http://netease-youdao.github.io/hex/downloads/hex_module_1.1.6_1621_windows32_debug_symbols)
     * [heX module release PDB](http://netease-youdao.github.io/hex/downloads/hex_module_1.1.6_1621_windows32_release_symbols)
   * [HeXium](http://netease-youdao.github.io/hex/downloads/hexium.1.1.6_1612.windows32.zip)
 * Mac OS X
   * [HeXium](http://netease-youdao.github.io/hex/downloads/hexium.1.1.6_1612.macosx32.zip)

The sources are coming soon!!!


## Authors
  * NetEase Youdao Information Technology (Beijing) Co.


## License

New BSD License

Copyright (c) 2012-2014 NetEase Youdao Inc. and other heX contributors.
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

