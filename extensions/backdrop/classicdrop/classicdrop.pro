top_srcdir  = ../../..
srcdir      = classicdrop

!include( $$top_srcdir/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

TEMPLATE = lib

QT += core declarative network opengl

DESTDIR = $$plexyext

INCLUDEPATH += $$top_srcdir/base/qt4 $$top_srcdir/base/shaders

CONFIG += qt

SOURCES = backdrop.cpp \
		classicinterface.cpp

HEADERS = backdrop.h \
		classicinterface.h

LIBS += -lplexyshaders -lplexydeskcore

TARGET = classicbackdrop

desktop_files.files = classic.desktop
desktop_files.path = $$sharedir/plexy/ext/groups

INSTALLS += desktop_files

target.path = $${plexyext}
INSTALLS += target