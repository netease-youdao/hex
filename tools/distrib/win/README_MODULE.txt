heX Module Binary Distribution
-------------------------------------------------------------------------------

Date:             $DATE$

heX Version:      $HEX_VER$
heX URL:          $HEX_URL$@$HEX_REV$

CEF Version:      $CEF_VER$
CEF URL:          $CEF_URL$@$CEF_REV$

Chromium Verison: $CHROMIUM_VER$
Chromium URL:     $CHROMIUM_URL$@$CHROMIUM_REV$


This distribution contains all components necessary to build and distribute an
application using heX Module. Please see the LICENSING section of this document
for licensing terms and conditions.


CONTENTS
--------

hexclient   Contains the hexclient sample application configured to build
            using the files in this distribution.

Debug       Contains libcef.dll, hex.dll and other components required to run
            the debug version of heX Module-based applications. Also acts as
            the build target for the Debug build of hexclient.

docs        Contains C++ API documentation generated from the CEF and heX
            header files.

include     Contains all required CEF and heX header files.

lib         Contains Debug and Release versions of the libcef.lib and hex.lib
            libraries file that all heX Module-based applications must link
            against.

libcef_dll  Contains the source code for the libcef_dll_wrapper static library
            that all applications using the CEF and heX C++ API must link
            against.

Release     Contains libcef.dll, hex.dll and other components required to run
            the release version of heX Module-based applications. Also acts as
            the build target for the Release build of hexclient.


USAGE
-----

Visual Studio 2012 and Visual Studio 2010:
  Open the hexclient2010.sln solution in Visual Studio and build.

Visual Studio 2008:
  Open the hexclient2008.sln solution in Visual Studio and build.

Visual Studio 2005:
  1. Open the hexclient.vcproj and libcef_dll_wrapper.vcproj files in a text
     editor. Change Version="9.00" to Version="8.00".
  2. Open the hexclient2005.sln file in a text editor. Change "Version 9.00" to
     "Version 8.00".
  3. Open the hexclient2005.sln solution in Visual Studio and build.

All heX's APIs are set to the "hex" object on global window object. Please see
API.txt for detail.

Please visit the CEF and heX Websites for additional usage information.

http://www.hexlib.org
http://code.google.com/p/chromiumembedded


REDISTRIBUTION
--------------

This binary distribution contains the below components. Components listed under
the "required" section must be redistributed with all applications using heX
Module. Components listed under the "optional" section may be excluded if the
related features will not be used.

Required components:

* CEF core library
    libcef.dll

* heX core library
    hex.dll

* V8 core library
    v8.dll
$NODE$
* Unicode support
    icudt.dll

Optional components:

* Localized resources
    locales/
  Note: Contains localized strings for WebKit UI controls. A .pak file is loaded
  from this folder based on the CefSettings.locale value. Only configured
  locales need to be distributed. If no locale is configured the default locale
  of "en-US" will be used. Locale file loading can be disabled completely using
  CefSettings.pack_loading_disabled. The locales folder path can be customized
  using CefSettings.locales_dir_path.

* Other resources
    cef.pak
    devtools_resources.pak
  Note: Contains WebKit image and inspector resources. Pack file loading can be
  disabled completely using CefSettings.pack_loading_disabled. The resources
  directory path can be customized using CefSettings.resources_dir_path.

* FFmpeg audio and video support
    ffmpegsumo.dll
  Note: Without this component HTML5 audio and video will not function.

* Angle and Direct3D support
    d3dcompiler_43.dll (required for Windows XP)
    d3dcompiler_46.dll (required for Windows Vista and newer)
    libEGL.dll
    libGLESv2.dll
  Note: Without these components HTML5 accelerated content like 2D canvas, 3D
  CSS and WebGL will not function.


LICENSING
---------

The heX project is BSD licensed. Please read the LICENSE.txt file included with
this binary distribution for licensing terms and conditions. The CEF project is
BSD licensed. Other software included in this distribution is provided under
other licenses. Please visit "about:credits" in a heX Module-based application
for complete Chromium and third-party licensing information.
