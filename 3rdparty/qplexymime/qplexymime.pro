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

DESTDIR = $${top_destdirlib}

TARGET = plexymime

LIBS += -lplexyshaders -lplexydeskcore

FREEDESKTOPXML = freedesktop.org.xml \
                mimetypes

freedesktop.files = $$FREEDESKTOPXML
freedesktop.path = $${sharedir}/plexy/mime/
INSTALLS += freedesktop
