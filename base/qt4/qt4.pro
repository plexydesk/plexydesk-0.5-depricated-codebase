top_srcdir  = ../..
srcdir = plexydeskcore

! include( $${top_srcdir}/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

TEMPLATE = lib

QT += core gui opengl declarative network svg

INCLUDEPATH += $${IN_PWD}/shaders

DEFINES += plexydeskcore_EXPORTS

win32:!wince*:DLLDESTDIR = $${top_destdirbin}

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
                svgprovider.cpp \
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
                svgprovider.h \
		themepackloader.h \
		viewlayer.h \
		widgetinterface.h \
		widgetplugin.h

LIBS += -lplexyshaders

TARGET = plexydeskcore

target.path = $${top_destdirlib}
INSTALLS += target
