top_srcdir  = ../../..
srcdir      = clock_widget

!include( $$top_srcdir/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

TEMPLATE = lib

QT += core declarative network opengl

DEFINES += plexyclock_EXPORTS

DESTDIR = $$plexyext

INCLUDEPATH += $$top_srcdir/base/qt4 $$top_srcdir/base/shaders

CONFIG += qt

SOURCES = clock.cpp \
        clockwidget.cpp

HEADERS = clock.h \
        clockinterface.h \
        clockwidget.h

CONFIG(debug, debug|release) {
    LIBS += -lplexyshadersd -lplexydeskcored
}

CONFIG(release, debug|release) {
    LIBS += -lplexyshaders -lplexydeskcore
}

TARGET = plexyclock

desktop_files.files = clock.desktop
desktop_files.path = $$top_destdir/share/plexy/ext/groups

SKIN_FILES = clock/background.png \
        clock/clock-shadow.png \
        clock/date_field.png \
        clock/face.png \
        clock/gloss.png \
        clock/hour-hand-long.png \
        clock/hour-hand.png \
        clock/mask.png \
        clock/minute-hand-long.png \
        clock/minute-hand.png \
        clock/second-hand-long.png \
        clock/second-hand.png \
        clock/thedot.png

clock_skin_files.files = $$SKIN_FILES
clock_skin_files.path = $$top_destdir/share/plexy/skins/default/clock

INSTALLS += clock_skin_files

INSTALLS += desktop_files

include($$top_srcdir/deploy.pri)
