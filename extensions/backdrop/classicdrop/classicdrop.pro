TEMPLATE = lib

QT += core declarative network opengl

DESTDIR = $${OUT_PWD}/../../build/lib/plexyext

INCLUDEPATH += $${OUT_PWD}/../../../ ../../../base/qt4 ../../../base/shaders ../../../base/core

CONFIG += qt

SOURCES = backdrop.cpp \
		classicinterface.cpp \
    classicbackgroundrender.cpp

HEADERS = backdrop.h \
		classicinterface.h \
    classicbackgroundrender.h

LIBS += -L$${OUT_PWD}/../../../build/lib -lplexyshaders -lplexydeskcore -lplexydeskuicore

TARGET = classicbackdrop

desktop_files.files = classic.desktop
desktop_files.path = $${OUT_PWD}/../../../build/lib/plexy/ext/groups

INSTALLS += desktop_files

target.path = $${OUT_PWD}/../../../build/lib/plexyext
INSTALLS += target
