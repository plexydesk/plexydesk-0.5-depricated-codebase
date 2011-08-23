top_srcdir  = ../..
srcdir      = mime

! include( $$top_srcdir/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

TEMPLATE = lib

QT += core declarative network xml

DEFINES += mimetype_EXPORTS

INCLUDEPATH += $$top_srcdir/base/qt4

CONFIG += qt

DESTDIR = $$top_destdir

SOURCES = freedesktopmime.cpp

HEADERS = freedesktopmime.h \
        mime_globals.h

LIBS += -lplexyshaders -lplexydeskcore

TARGET = mimetype

target.path = $$top_destdir
INSTALLS += target
