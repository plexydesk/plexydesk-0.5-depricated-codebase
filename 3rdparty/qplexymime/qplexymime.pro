top_srcdir  = ../..
srcdir      = qplexymime

! include( $${top_srcdir}/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

TEMPLATE = lib

QT += core declarative network xml xmlpatterns

DEFINES += plexymime_EXPORTS

INCLUDEPATH += $${top_srcdir}/base/qt4

win32:!wince*:DLLDESTDIR = $${top_destdirbin}

CONFIG += qt

SOURCES = qplexymime.cpp

HEADERS = qplexymime.h

RESOURCES = qplexymime.qrc

LIBS += -lplexyshaders -lplexydeskcore

TARGET = plexymime

target.path = $${top_destdirlib}
INSTALLS += target
