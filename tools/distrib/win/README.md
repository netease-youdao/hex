heX Binary Distribution
-------------------------------------------------------------------------------

Date:             $DATE$

heX Version:      $HEX_VER$
heX URL:          $HEX_URL$@$HEX_REV$

CEF Version:      $CEF_VER$
CEF URL:          $CEF_URL$@$CEF_REV$

Chromium Verison: $CHROMIUM_VER$
Chromium URL:     $CHROMIUM_URL$@$CHROMIUM_REV$


This distribution contains all components necessary to build and distribute an
application using heX. Please see the LICENSING section of this document for
licensing terms and conditions.


CONTENTS
--------

hexclient.exe           The main executable file of heX, this is the entry of
                        the application.

manifest.json           A JSON-formatted manifest file, that provides important
                        information of the application. Please see the MANIFEST
                        section of this document for detail.

cef.pak                 Contains WebKit image and inspector resources.
hex_resources.pak

libcef.dll              CEF core library.

hex.dll                 heX core library.

v8.dll                  V8 core library.
$NODE$
icudt.dll               Unicode support.

The files below is optional:

locales/*               Contains localized strings for WebKit UI controls.
                        Note: "locale" must be empty string in manifest.json.

libEGL.dll              Angle and Direct3D support. Note: Without these
libGLESv2.dll           components HTML5 accelerated content like 2D canvas, 3D
                        CSS and WebGL will not function.
d3dcompiler_43.dll          (required for Windows XP)
d3dcompiler_46.dll          (required for Windows Vista and newer)

ffmpegsumo.dll          FFmpeg audio and video support. Without this component
                        HTML5 audio and video will not function.


MANIFEST
--------

{
  "first_page": "$(AppDir)heX.html",    // The homepage of the application.
                                        // $(AppDir) indicates the application
                                        // directory.
                                        // Default value: "$(AppDir)heX.html"

  "application_title": "heX",           // The default title of the application.
                                        // This is useful for alert, confirm or
                                        // prompt dialog.
                                        // Default value: "heX"

  "version": "1.0",                     // The version info of the application.
                                        // Default value ""

  "locale": "zh-CN",                    // The locale for WebKit. Note: Empty
                                        // string indicates that the locale
                                        // directory and all pak files can be
                                        // removed.
                                        // Default value: ""

  "multiple_process": false,            // Set to true to launch application in
                                        // Chromium's multiple process mode.
                                        // Default value: false

  "disable_gpu": false,                 // Set to true to disable GPU
                                        // acceleration.
                                        // Default value: false

  "launch_node_in_all_pages": true,     // Set to true to launch Node.JS in all
                                        // pages including the popup window. Or
                                        // only the main window can use Node.JS.
                                        // Default value: false

  "use_node_typed_arrays": false,       // Set to true to replace Chromium's
                                        // typed-arrays by Node.JS'. Or Node.JS'
                                        // typed-arrays must be used by
                                        // require('typed-arrays').
                                        // Default value: false

  "form": {
    "borderless": false,                // Set to true to create a borderless,
                                        // captionless customized form.
                                        // Default value: false

    "no_system_animation": false,       // Set to true to disable system
                                        // animation on form. Note: Only
                                        // available when "borderless" is true.
                                        // Default value: false

    "no_system_shadow": false,          // Set to true to disable system shadow
                                        // on form. Note: Only available when
                                        // "borderless" is true.
                                        // Default value: false

    "disable_form_apis": false,         // Set to true to disable all form APIs.
                                        // Default value: false

    "opacity": "none",                  // The opacity of the form. "none"
                                        // indicates disabled. The number 0
                                        // indicates transparency, and 255
                                        // indicates opacity.
                                        // Default value: "none"

    "hook_system_command": false,       // Set to true to hook the system
                                        // commands when triggered, then we
                                        // must execute the system commands
                                        // manually.
                                        // Default value: false

    "hide_in_taskbar": false,           // Set to true to hide the application
                                        // button in taskbar. Note: Only
                                        // available when "borderless" is true.
                                        // Default value: false

    "launch_state": "normal",           // The initial state when application is
                                        // launching. The value can be "normal",
                                        // "minimized", "maximized", "hidden",
                                        // "fullscreen", and the string is
                                        // case-insensitive.
                                        // Default value: "normal"

    "top_most": false,                  // Set to true to make the form be the
                                        // top-most.
                                        // Default value: false

    "launch_width": 800,                // The initial width of the form when
                                        // launching. The value of this property
                                        // should be number usually. If the
                                        // value is case-insensitive string
                                        // "system_default" or the number -1, it
                                        // indicates that the width will be the
                                        // calculated by system.
                                        // Default value: SYSTEM_DEFAULT

    "launch_height": 600,               // The same as "launch_width".
                                        // Default value: SYSTEM_DEFAULT

    "launch_x": "screen_centered",      // The initial x-axis position of the
                                        // form when launching. The value of
                                        // this property should be number
                                        // usually. If the value is case-
                                        // insensitive string "system_default"
                                        // or the number -1, it indicates that
                                        // the x-axis position will be the
                                        // calculated by system. The string
                                        // "parent_centered" or the number -2
                                        // indicates that the x-axis position
                                        // will be the center of the parent form
                                        // if has. And the string
                                        // "screen-centered" or the number -3
                                        // indicates that the x-axis position
                                        // will be the center of the active
                                        // screen.
                                        // Default value: SYSTEM_DEFAULT

    "launch_y": "screen_centered",      // The same as "launch_x".
                                        // Default value: SYSTEM_DEFAULT

    "min_width": 0,                     // The min width of the form when
                                        // sizing. The value of this property
                                        // must be number and greater than 0.
                                        // Default value: 0

    "min_height": 0,                    // The same as "min_width".
                                        // Default value: 0

    "max_width": 0,                     // The same as "min_width".
                                        // Default value: 0

    "max_height": 0,                    // The same as "min_width".
                                        // Default value: 0

    "border_width": 0                   // The imaginary border width of the
                                        // form when "borderless" is true. The
                                        // value must be positive integer.
                                        // Default value: 5
  },
  "cache_path": "data",                 // The location where cache data stored
                                        // on disk. If empty an in-memory cache
                                        // will be used. HTML5 databases such as
                                        // localStorage will only persist across
                                        // sessions if a cache path is
                                        // specified.
                                        // Default value: ""

  "remote_debugging_port": 54321,       // Set to a value between 1024 and 65535
                                        // to enable remote debugging on the
                                        // specified port. For example, if 54321
                                        // is specified the remote debugging URL
                                        // will be http://localhost:54321. If
                                        // port number is occupied, this value
                                        // will be ignored.
                                        // Default value: 0

  "user_agent": "hex"                   // Value that will be returned as the
                                        // User-Agent HTTP header. If empty the
                                        // default User-Agent string will be
                                        // used.
                                        // Default value: ""

  "browser": {
    // Controls whether web security restrictions (same-origin policy) will be
    // enforced. Disabling this setting is not recommend as it will allow risky
    // security behavior such as cross-site scripting (XSS).
    // Default value: false
    "web_security_disabled": true,

    // Controls whether file URLs will have access to other file URLs.
    // Default value: false
    "file_access_from_file_urls_allowed": true,

    // Controls whether file URLs will have access to all URLs.
    // Default value: false
    "universal_access_from_file_urls_allowed": true
  }
}


USAGE
-----

Just run hexclient.exe. Enjoy!

All heX's APIs are set to the "hex" object on global window object. Please see
API.txt for detail.

Please visit the heX Websites for additional usage information.

http://hex.youdao.com


LICENSING
---------

The heX project is BSD licensed. Please read the LICENSE.txt file included with
this binary distribution for licensing terms and conditions. The CEF project is
BSD licensed. Other software included in this distribution is provided under
other licenses. Please visit "about:credits" for complete Chromium and
third-party licensing information.
