top_srcdir  = ..
srcdir      = runner

! include( ../common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

TEMPLATE = app

QT += core gui opengl declarative network xml xmlpatterns

INCLUDEPATH += $$top_srcdir/base/qt4 $$top_srcdir/base/qt4/shaders $$top_srcdir/3rdparty/qplexymime

SOURCES += desktopview.cpp \
           icon.cpp \
           iconjob.cpp \
           iconprovider.cpp \
           main.cpp \
           plexyeventhandler.cpp \
           plexypanel.cpp \
           plexytray.cpp

HEADERS += desktopview.h \
           icon.h \
           iconjob.h \
           iconprovider.h \
           plexyeventhandler.h \
           plexypanel.h \
           plexytray.h

unix {
    SOURCES += plexywindow.cpp

    HEADERS += plexywindow.h \
            XAtoms.h
}

LIBS += -L../base/qt4 -lplexyeskcore
#LIBS += -L../logic -L../gui -llogic -lgui

TARGET = runner
