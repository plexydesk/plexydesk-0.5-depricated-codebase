top_srcdir  = ../..
srcdir      = qplexymime

! include( $$top_srcdir/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

TEMPLATE = lib

QT += core declarative network xml xmlpatterns

DEFINES += plexymime_EXPORTS

INCLUDEPATH += $$top_srcdir/base/qt4

CONFIG += qt

DESTDIR = $$top_destdir

SOURCES = qplexymime.cpp

HEADERS = qplexymime.h

RESOURCES = qplexymime.qrc

CONFIG(debug, debug|release) {
    LIBS += -lplexyshadersd -lplexydeskcored
}

CONFIG(release, debug|release) {
    LIBS += -lplexyshaders -lplexydeskcore
}

TARGET = plexymime

include($$top_srcdir/deploy.pri)
