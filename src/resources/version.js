'use strict';
function TitleBar (id, className) {
  var This = this;
  this.id = id || '';
  this.object = document.getElementById(id);
  if (!this.object) {
    throw new Error('DOM element cannot find.');
  }

  if (!TitleBar.initialized) {
    if (!hex.aeroGlassEnabled) {
      TitleBar.bodyElement.classList.add(TitleBar.aeroDisabled);
      TitleBar.rootElement.classList.add(TitleBar.aeroDisabled);
      this.object.classList.add(TitleBar.aeroDisabled);
      TitleBar.bodyElement.classList.remove(TitleBar.aeroEnabled);
      TitleBar.rootElement.classList.remove(TitleBar.aeroEnabled);
      this.object.classList.remove(TitleBar.aeroEnabled);
    } else {
      TitleBar.bodyElement.classList.remove(TitleBar.aeroDisabled);
      TitleBar.rootElement.classList.remove(TitleBar.aeroDisabled);
      this.object.classList.remove(TitleBar.aeroDisabled);
      TitleBar.bodyElement.classList.add(TitleBar.aeroEnabled);
      TitleBar.rootElement.classList.add(TitleBar.aeroEnabled);
      this.object.classList.add(TitleBar.aeroEnabled);
    }

    var getTitleBarConditions = function (e) {
      var conditions = false;
      if (!This.areaElementSelectors) {
        return true;
      }
      var includings = This.areaElementSelectors.include;
      var excludings = This.areaElementSelectors.exclude;
      Object.keys(includings).forEach(function(element, index, list) {
        var object = (element == 'this') ? This.object : includings[element];
        var subCondition = object.contains(e.target);
        if (excludings[element]) {
          subCondition = subCondition && e.target != excludings[element];
        }
        conditions = conditions || subCondition;
      });
      return conditions;
    }

    document.addEventListener('mousemove', function (e) {
      if (getTitleBarConditions(e)) {
        hex.setAsTitleBarAreas(e.clientX, e.clientY);
        e.stopPropagation();
      } else {
        hex.setAsTitleBarAreas(-1, -1);
        hex.setAsNonBorderAreas(-1, -1);
      }
      if (e.target == system_icon) {
        hex.setAsSystemMenuIconAreas(e.clientX, e.clientY);
        e.stopPropagation();
      } else {
        hex.setAsSystemMenuIconAreas(-1, -1);
      }
    }, false);

    document.addEventListener('mouseup', function (e) {
      if (e.button != 2)
        return;
      if (getTitleBarConditions(e)) {
        hex.showSystemMenu(e.clientX, e.clientY);
      }
    }, false);

    TitleBar.initialized = true;
  }

  this.object.object = this;
  this.className = className || '';
}
TitleBar.initialize = function () {
  document.addEventListener('contextmenu', function (e) {
    e.preventDefault();
  }, false);

  window.addEventListener('formActivated', function (e) {
    if (e.detail) {
      TitleBar.bodyElement.classList.remove(TitleBar.inactive);
      TitleBar.rootElement.classList.remove(TitleBar.inactive);
      TitleBar.bodyElement.classList.add(TitleBar.active);
      TitleBar.rootElement.classList.add(TitleBar.active);
    } else {
      TitleBar.bodyElement.classList.add(TitleBar.inactive);
      TitleBar.rootElement.classList.add(TitleBar.inactive);
      TitleBar.bodyElement.classList.remove(TitleBar.active);
      TitleBar.rootElement.classList.remove(TitleBar.active);
    }
  }, false);

  document.addEventListener('mousemove', function (e) {
      hex.setAsNonBorderAreas(-1, -1);
  }, false);
  
  TitleBar.rootElement.style.padding = (5) + 'px';
}
Object.defineProperty(TitleBar, 'rootElement', {
  get: function () {
    return document.documentElement;
  }
});
Object.defineProperty(TitleBar, 'bodyElement', {
  get: function () {
    return document.body;
  }
});
Object.defineProperty(TitleBar.prototype, 'areaElementSelectors', {
  get: function () {
    return this.areaSelectors;
  },
  set: function (selectors) {
    this.areaSelectors = selectors;
  },
});
Object.defineProperty(TitleBar, 'active', {
  get: function () {
    return this.activeClass;
  },
  set: function (className) {
    this.activeClass = className;
  },
});
Object.defineProperty(TitleBar, 'inactive', {
  get: function () {
    return this.inactiveClass;
  },
  set: function (className) {
    this.inactiveClass = className;
  },
});
Object.defineProperty(TitleBar, 'aeroDisabled', {
  get: function () {
    return this.aeroDisabledClass;
  },
  set: function (className) {
    this.aeroDisabledClass = className;
  },
});
Object.defineProperty(TitleBar, 'aeroEnabled', {
  get: function () {
    return this.aeroEnabledClass;
  },
  set: function (className) {
    this.aeroEnabledClass = className;
  },
});


function ControlBar (id, className) {
  var This = this;
  this.id = id || '';
  this.object = document.getElementById(id);
  if (!this.object) {
    throw new Error('DOM element cannot find.');
  }
  this.object.object = this;

  if (!ControlBar.initialized) {
    ControlBar.NORMAL = 0;
    ControlBar.HOVER = 1;
    ControlBar.PRESSED = 2;
    ControlBar.className = className;

    if (!hex.aeroGlassEnabled) {
      This.object.classList.add(TitleBar.aeroDisabled);
      This.object.classList.remove(TitleBar.aeroEnabled);
    } else {
      This.object.classList.remove(TitleBar.aeroDisabled);
      This.object.classList.add(TitleBar.aeroEnabled);
    }

    window.addEventListener('formAeroChanged', function (e) {
      if (!e.detail) {
        TitleBar.bodyElement.classList.add(TitleBar.aeroDisabled);
        TitleBar.rootElement.classList.add(TitleBar.aeroDisabled);
        This.object.classList.add(TitleBar.aeroDisabled);
        This.titleBar.object.classList.add(TitleBar.aeroDisabled);
        TitleBar.bodyElement.classList.remove(TitleBar.aeroEnabled);
        TitleBar.rootElement.classList.remove(TitleBar.aeroEnabled);
        This.object.classList.remove(TitleBar.aeroEnabled);
        This.titleBar.object.classList.remove(TitleBar.aeroEnabled);
      } else {
        TitleBar.bodyElement.classList.remove(TitleBar.aeroDisabled);
        TitleBar.rootElement.classList.remove(TitleBar.aeroDisabled);
        This.object.classList.remove(TitleBar.aeroDisabled);
        This.titleBar.object.classList.remove(TitleBar.aeroDisabled);
        TitleBar.bodyElement.classList.add(TitleBar.aeroEnabled);
        TitleBar.rootElement.classList.add(TitleBar.aeroEnabled);
        This.object.classList.add(TitleBar.aeroEnabled);
        This.titleBar.object.classList.add(TitleBar.aeroEnabled);
      }
    }, false);

    ControlBar.initialized = true;
  }

  this.className = className || '';
  this.state = ControlButton.NORMAL;
}
Object.defineProperty(ControlBar.prototype, 'maximized', {
  get: function () {
    return this.maximizedClass;
  },
  set: function (className) {
    this.maximizedClass = className;
  },
});
Object.defineProperty(ControlBar.prototype, 'titleBar', {
  get: function () {
    return this.titleBarObject;
  },
  set: function (bar) {
    this.titleBarObject = bar;
  },
});


function ControlButton (id, type, className) {
  var This = this;
  this.id = id || '';
  this.object = document.getElementById(id);
  if (!this.object) {
    throw new Error('DOM element cannot find.');
  }
  this.object.object = this;

  if (!ControlButton.initialized) {
    ControlButton.NORMAL = 0;
    ControlButton.HOVER = 1;
    ControlButton.PRESSED = 2;
    ControlButton.className = className;

    ControlButton.initialized = true;
  }

  this.type = type | 0;
  this.className = className || '';
  this.state = ControlButton.NORMAL;
  switch (this.type) {
    case hex.MINIMIZE:
      ControlButton.minButton = this;
      break;
    case hex.MAXIMIZE:
      ControlButton.maxButton = this;
      break;
    case hex.CLOSE:
      ControlButton.closeButton = this;
      break;
  }
}
ControlButton.initialize = function () {
  document.addEventListener('mouseup', function (e) {
    ControlButton.maxButton.state = ControlButton.maxButton.controlBar.state = ControlButton.NORMAL;
    ControlButton.minButton.state = ControlButton.minButton.controlBar.state = ControlButton.NORMAL;
    ControlButton.closeButton.state = ControlButton.closeButton.controlBar.state = ControlButton.NORMAL;
  }, false);

  window.addEventListener('formResized', function (e) {
    switch (e.detail) {
      case hex.RESTORE:
        ControlButton.maxButton.object.classList.remove(ControlButton.maxButton.restore);
        ControlButton.maxButton.type = hex.MAXIMIZE;
        ControlButton.maxButton.titleBar.object.classList.remove(ControlButton.maxButton.maximized);
        ControlButton.maxButton.controlBar.object.classList.remove(ControlButton.maxButton.maximized);
        TitleBar.rootElement.style.padding = (5) + 'px';
        break;
      case hex.MAXIMIZE:
        ControlButton.maxButton.object.classList.add(ControlButton.maxButton.restore);
        ControlButton.maxButton.type = hex.RESTORE;
        ControlButton.maxButton.titleBar.object.classList.add(ControlButton.maxButton.maximized);
        ControlButton.maxButton.controlBar.object.classList.add(ControlButton.maxButton.maximized);
        TitleBar.rootElement.style.padding = (hex.getMaximizedAdjustedBorderWidth()) + 'px'
        break;
    }
  }, false);

  window.addEventListener('formMouseLeave', function (e) {
    var event = document.createEvent('MouseEvent');
    event.initMouseEvent('mouseout', true, true, window, 0, 0, 0, 0, 0, false, false, false, false, 0, null);
    ControlButton.closeButton.object.dispatchEvent(event);
    ControlButton.maxButton.object.dispatchEvent(event);
    ControlButton.minButton.object.dispatchEvent(event);
  }, false);
}
Object.defineProperty(ControlButton.prototype, 'normal', {
  get: function () {
    return this.normalClass;
  },
  set: function (className) {
    this.normalClass = className;
  },
});
Object.defineProperty(ControlButton.prototype, 'hover', {
  get: function () {
    return this.hoverClass;
  },
  set: function (className) {
    this.hoverClass = className;
  },
});
Object.defineProperty(ControlButton.prototype, 'pressed', {
  get: function () {
    return this.pressedClass;
  },
  set: function (className) {
    this.pressedClass = className;
  },
});
Object.defineProperty(ControlButton.prototype, 'maximized', {
  get: function () {
    return this.maximizedClass;
  },
  set: function (className) {
    this.maximizedClass = className;
  },
});
Object.defineProperty(ControlButton.prototype, 'restore', {
  get: function () {
    return this.restoreClass;
  },
  set: function (className) {
    this.restoreClass = className;
  },
});
Object.defineProperty(ControlButton.prototype, 'controlBar', {
  get: function () {
    return this.controlBarObject;
  },
  set: function (bar) {
    this.controlBarObject = bar;
  },
});
Object.defineProperty(ControlButton.prototype, 'titleBar', {
  get: function () {
    return this.titleBarObject;
  },
  set: function (bar) {
    this.titleBarObject = bar;
  },
});
ControlButton.prototype.initializeEvent = function () {
  var This = this;
  var onMouseOver = function (e) {
    this.classList.remove(this.object.pressed);
    this.classList.remove(this.object.hover);
    if (this.object.state == this.object.constructor.PRESSED) {
      this.classList.add(this.object.pressedClass);
    }
    if (this.object.controlBar.state != this.object.constructor.PRESSED) {
      this.classList.add(this.object.hoverClass);
    }
  }
  var onMouseOut = function (e) {
    this.classList.remove(this.object.hover);
    this.classList.remove(this.object.pressed);
  }
  var onMouseDown = function (e) {
    if (e.button != 0)
      return;
    this.classList.add(this.object.pressed);
    this.object.state = this.object.controlBar.state = this.object.constructor.PRESSED;
  }
  var onMouseUp = function (e) {
    if (e.button != 0)
      return;
    this.classList.remove(this.object.pressed);
    this.object.state = this.object.controlBar.state = this.object.constructor.HOVER;
  }
  var onClick = function (e) {
    if (e.button != 0)
      return;
    switch (this.object.type) {
      case hex.MINIMIZE:
        hex.minimize();
        break;
      case hex.CLOSE:
        hex.close();
        break;
      case hex.MAXIMIZE:
        hex.maximize();
        break;
      case hex.RESTORE:
        hex.restore();
        break;
    }
    this.classList.remove(this.object.pressed);
  }
  var onMouseMove = function (e) {
    hex.setAsTitleBarAreas(-1, -1);
    hex.setAsNonBorderAreas(e.clientX, e.clientY);
    e.stopPropagation();
  }
  this.object.addEventListener('mouseover', onMouseOver, false);
  this.object.addEventListener('mouseout', onMouseOut, false);
  this.object.addEventListener('mousedown', onMouseDown, false);
  this.object.addEventListener('mouseup', onMouseUp, false);
  this.object.addEventListener('mousemove', onMouseMove, false);
  this.object.addEventListener('click', onClick, false);
}


TitleBar.aeroEnabled = 'aero';
TitleBar.aeroDisabled = 'non_aero';
TitleBar.active = 'active';
TitleBar.inactive = 'inactive';
TitleBar.initialize();
ControlButton.initialize();


