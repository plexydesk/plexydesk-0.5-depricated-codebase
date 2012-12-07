TEMPLATE = lib

QT += core declarative network xml xmlpatterns

DEFINES += plexymime_EXPORTS

DESTDIR = $${OUT_PWD}/../../build/lib
INCLUDEPATH += ../../base/qt4 ../../base/core

win32:!wince*:DLLDESTDIR = $${OUT_PWD}/../../build/bin

CONFIG += qt

SOURCES = qplexymime.cpp

HEADERS = qplexymime.h

RESOURCES = qplexymime.qrc

TARGET = plexymime

LIBS += -L./../../build/lib -lplexyshaders -lplexydeskuicore

FREEDESKTOPXML = freedesktop.org.xml \
                mimetypes

freedesktop.files = $$FREEDESKTOPXML
freedesktop.path = $${OUT_PWD}/../../build/share/plexy/mime/
INSTALLS += freedesktop
