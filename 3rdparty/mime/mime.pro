TEMPLATE = lib

QT += core declarative network xml

DEFINES += mimetype_EXPORTS

INCLUDEPATH += ../../base/qt4 ../../base/core

CONFIG += qt

DESTDIR = $${OUT_PWD}/../../build/lib
win32:!wince*:DLLDESTDIR = $${OUT_PWD}../../build/bin

SOURCES = freedesktopmime.cpp

HEADERS = freedesktopmime.h \
        mime_globals.h

LIBS += -L./../../build/lib -lplexyshaders -lplexydeskcore -lplexydeskuicore

TARGET = mimetype

target.path = $${OUT_PWD}/../../build/lib
INSTALLS += target
