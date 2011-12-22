top_srcdir  = ../..
srcdir      = fbjson

!include( $$top_srcdir/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

TEMPLATE = lib

QT += core script

DESTDIR = $$top_destdirlib

INCLUDEPATH += $$top_srcdir/base/qt4 $$top_srcdir/base/shaders

CONFIG += qt

DEFINES += plexyjson_EXPORTS

SOURCES = jsonhandler.cpp

HEADERS = json_global.h \
		jsonhandler.h

TARGET = plexyjson
target.path = $$top_destdirlib
INSTALLS += target
