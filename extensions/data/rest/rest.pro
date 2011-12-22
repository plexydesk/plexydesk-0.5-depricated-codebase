top_srcdir  = ../../..
srcdir      = restengine

!include( $$top_srcdir/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

TEMPLATE = lib

QT += core network declarative opengl

LIBS += -lplexyshaders -lplexydeskcore

DESTDIR = $$plexyext

INCLUDEPATH += $$top_srcdir/base/qt4 $$top_srcdir/base/shaders

CONFIG += qt

SOURCES = rest.cpp \
		restinterface.cpp

HEADERS = rest.h \
		restinterface.h

TARGET = restengine

desktop_files.files = rest.desktop
desktop_files.path = $$sharedir/plexy/ext/groups

INSTALLS += desktop_files

target.path = $$plexyext
INSTALLS += target
