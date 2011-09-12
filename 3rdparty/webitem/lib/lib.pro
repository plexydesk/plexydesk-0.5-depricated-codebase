top_srcdir  = ../../..
srcdir      = webitemlib

! include( $${top_srcdir}/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

TEMPLATE = lib

QT += core webkit

DEFINES += webqgv_EXPORTS

INCLUDEPATH += $$top_srcdir/base/qt4

CONFIG += qt

win32:!wince*:DLLDESTDIR = $${top_destdirbin}

SOURCES = qwebviewitem.cpp

HEADERS = qwebviewitem.h

TARGET = webqgv

target.path = $${top_destdirlib}
INSTALLS += target
