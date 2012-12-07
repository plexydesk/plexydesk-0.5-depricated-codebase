TEMPLATE = lib

QT += core declarative network opengl

DEFINES += plexyclock_EXPORTS

DESTDIR = $${OUT_PWD}/../../../build/lib/plexyext

INCLUDEPATH += ../../../base/qt4 ../../../base/shaders ../../../base/core

CONFIG += qt

SOURCES = clock.cpp \
        clockwidget.cpp

HEADERS = clock.h \
        clockinterface.h \
        clockwidget.h

LIBS += -L$${OUT_PWD}/../../../build/lib -lplexyshaders -lplexydeskcore -lplexydeskuicore

TARGET = plexyclock

desktop_files.files = clock.desktop
desktop_files.path = $${OUT_PWD}/../../../build/lib/plexy/ext/groups

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
clock_skin_files.path = $${OUT_PWD}/../../../build/plexy/skins/default/clock

INSTALLS += clock_skin_files
INSTALLS += desktop_files

