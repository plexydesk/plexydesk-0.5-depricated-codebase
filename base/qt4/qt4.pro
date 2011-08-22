top_srcdir  = ../..
srcdir      = plexydeskcore

! include( $$top_srcdir/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

TEMPLATE = lib

QT += core gui opengl declarative network svg

INCLUDEPATH += shaders

DEFINES += plexydeskcore_EXPORTS

CONFIG += qt

SOURCES = abstractplugininterface.cpp \
		backdropinterface.cpp \
		backdropplugin.cpp \
		baserender.cpp \
		desktopwidget.cpp \
		extensionfactory.cpp \
		imagecache.cpp \
		netwm.cpp \
		pendingjob.cpp \
		plexyconfig.cpp \
		pluginloader.cpp \
		themepackloader.cpp \
		viewlayer.cpp

HEADERS = abstractplugininterface.h \
		backdropinterface.h \
		backdropplugin.h \
		baseplugin.h \
		baserender.h \
		datainterface.h \
		dataplugin.h \
		debug.h \
		desktopwidget.h \
		extensionfactory.h \
		imagecache.h \
		netwm.h \
		netwm_def.h \
		netwm_p.h \
		pendingjob.h \
		plexy.h \
		plexyconfig.h \
		pluginloader.h \
		socialinterface.h \
		socialplugin.h \
		themepackloader.h \
		viewlayer.h \
		widgetinterface.h \
		widgetplugin.h

LIBS += -L$$top_destdir -lplexyshaders

TARGET = plexydeskcore

