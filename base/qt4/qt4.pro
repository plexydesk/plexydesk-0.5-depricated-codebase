TEMPLATE = lib

QT += core gui opengl declarative network svg xml

INCLUDEPATH += $${OUT_PWD}/../../ shaders ../core

DEFINES += plexydeskuicore_EXPORTS

DESTDIR = $${OUT_PWD}/../../build/lib
win32:!wince*:DLLDESTDIR = $${OUT_PWD}/../../build/bin

CONFIG += qt

SOURCES = viewlayer.cpp \
        themepackloader.cpp \
        svgprovider.cpp \
        style.cpp \
        scrollwidget.cpp \
        qmlsvgprovider.cpp \
        qmlpixmapprovider.cpp \
        qmldesktopwidget.cpp \
        plexyqmlglue.cpp \
        plexyconfig.cpp \
        nativestyle.cpp \
        lineedit.cpp \
        imagecache.cpp \
        extensionfactory.cpp \
        desktopwidget.cpp \
        button.cpp \
        baserender.cpp

HEADERS = viewlayer.h \
        themepackloader.h \
        svgprovider.h \
        style.h \
        socialplugin.h \
        socialinterface.h \
        scrollwidget.h \
        qmlsvgprovider.h \
        qmlpixmapprovider.h \
        qmldesktopwidget.h \
        plexyqmlglue.h \
        plexyconfig.h \
        nativestyle.h \
        lineedit.h \
        imagecache.h \
        extensionfactory.h \
        desktopwidget.h \
        button.h \
        baserender.h \
        plexydeskuicore_global.h

unix:SOURCES += netwm.cpp
unix:HEADERS += netwm_p.h \
        netwm_def.h \
        netwm.h \

LIBS += -L./../../build/lib -lplexyshaders -lplexydeskcore

TARGET = plexydeskuicore

