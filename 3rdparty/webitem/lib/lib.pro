TEMPLATE = lib

QT += core webkit

DEFINES += webqgv_EXPORTS

INCLUDEPATH += ../../../base/qt4

CONFIG += qt

DESTDIR = $${OUT_PWD}/../../../build/lib
win32:!wince*:DLLDESTDIR = $${OUT_PWD}/../../../build/bin

SOURCES = qwebviewitem.cpp

HEADERS = qwebviewitem.h

TARGET = webqgv

target.path = $${OUT_PWD}/../../../build/lib
INSTALLS += target
