TEMPLATE = lib
CONFIG  += dll plugin

INCLUDEPATH     += $$PWD
DEPENDPATH      += $$PWD

SOURCES	       += $$PWD/qtbrowserplugin.cpp
HEADERS        += $$PWD/qtbrowserplugin.h qtnpapi.h

win32 {
  SOURCES     += $$PWD/qtbrowserplugin_win.cpp
  !isEmpty(TARGET) {
    TARGET = np$$TARGET
  }
  LIBS        += -luser32

  qaxserver {
    DEF_FILE       += $$PWD/qtbrowserpluginax.def
  } else {
    DEF_FILE       += $$PWD/qtbrowserplugin.def
  }

  firefox {
    exists("c:/program files/mozilla firefox/plugins") {
      DLLDESTDIR += "c:/program files/mozilla firefox/plugins"
    } else {
      message("Firefox not found at default location")
    }
  }
  opera {
    exists("c:/program files/opera/program/plugins") {
      DLLDESTDIR += "c:/program files/opera/program/plugins"
    } else {
      message("Opera not found at default location")
    }
  }
  netscape {
    exists("c:/program files/netscape/netscape browser/plugins") {
      DLLDESTDIR += "c:/program files/netscape/netscape browser/plugins"
    } else {
      message("Netscape not found at default location")
    }
  }
} else:mac {
  CONFIG += plugin_bundle
  SOURCES     += $$PWD/qtbrowserplugin_mac.cpp
  #target.path = /Library/Internet\ Plugins
  #INSTALLS += target
} else {
  SOURCES     += $$PWD/qtbrowserplugin_x11.cpp
  INCLUDEPATH += /usr/X11R6/include
  
  # Uncomment the line below to avoid symbol clash with other Qt
  # versions in the browser (ref. developingplugins.html in the doc.).
  # Makes the dynamic linker prefer our own Qt symbols for the plugin,
  # provided that our Qt is statically built and linked into the
  # plugin. Note that to force the linker to prefer the static Qt
  # libraries (.a files), the dynamic libraries (.so) files must not
  # be present in the lib directory.
  #
  # QMAKE_LFLAGS += -Wl,-Bsymbolic
}
