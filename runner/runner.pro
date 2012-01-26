top_srcdir  = ..
srcdir      = runner

! include( ../common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

TEMPLATE = app

QT += core gui opengl declarative network xml xmlpatterns

# TODO:
#RC_FILE = $$top_srcdir/dist/windows/plexydesk.rc

DESTDIR = $${top_destdirbin}

INCLUDEPATH += $${top_srcdir}/base/qt4 $${top_srcdir}/base/qt4/shaders $${top_srcdir}/3rdparty/qplexymime

SOURCES += desktopbaseui.cpp \
           desktopview.cpp \
           icon.cpp \
           iconjob.cpp \
           iconprovider.cpp \
           main.cpp \
           plexyeventhandler.cpp \
           plexypanel.cpp \
           plexytray.cpp

HEADERS += desktopbaseui.h \
           desktopview.h \
           icon.h \
           iconjob.h \
           iconprovider.h \
           plexyeventhandler.h \
           plexypanel.h \
           plexytray.h

unix:!macx {
    SOURCES += plexywindow.cpp

    HEADERS += plexywindow.h \
            XAtoms.h
}

LIBS += -lplexymime -lplexyshaders -lplexydeskcore

TARGET = plexydesk

target.path = $${top_destdirbin}
INSTALLS += target
