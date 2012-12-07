TEMPLATE = lib

QT += core network declarative opengl

LIBS += -L$${OUT_PWD}/../../../build/lib -lplexyshaders -lplexydeskcore -lplexydeskuicore

DESTDIR = $${OUT_PWD}/../../../build/lib/plexyext

INCLUDEPATH += $${OUT_PWD}/../../../ ../../../base/qt4 ../../../base/shaders ../../../base/core

CONFIG += qt

SOURCES = rest.cpp \
		restinterface.cpp

HEADERS = rest.h \
		restinterface.h

TARGET = restengine

desktop_files.files = rest.desktop
desktop_files.path = $${OUT_PWD}/../../../build/lib/plexy/ext/groups

INSTALLS += desktop_files

target.path = $${OUT_PWD}/../../../build/lib/plexyext
INSTALLS += target
