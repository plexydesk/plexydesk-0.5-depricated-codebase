TEMPLATE = lib

QT += core script

DESTDIR = $${OUT_PWD}/../../../build/lib

INCLUDEPATH += ../../../base/qt4 ../../../base/shaders

CONFIG += qt

DEFINES += plexyjson_EXPORTS

SOURCES = jsonhandler.cpp

HEADERS = json_global.h \
		jsonhandler.h

TARGET = plexyjson
target.path = $${OUT_PWD}/../../../build/lib
INSTALLS += target
