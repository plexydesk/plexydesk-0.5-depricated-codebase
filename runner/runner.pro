TEMPLATE = app

QT += core gui opengl declarative network xml xmlpatterns

# TODO:
#RC_FILE = $$top_srcdir/dist/windows/plexydesk.rc

DESTDIR = $${OUT_PWD}/../build/bin
win32:!wince*:DLLDESTDIR = $${OUT_PWD}/../../build/bin

INCLUDEPATH += $${OUT_PWD}/.. ../base/qt4 ../base/qt4/shaders ../3rdparty/qplexymime \
    ../base/core

SOURCES += plexypanel.cpp \
    plexyeventhandler.cpp \
    main.cpp \
    iconprovider.cpp \
    iconjob.cpp \
    icon.cpp \
    desktopbaseui.cpp

HEADERS += plexypanel.h \
    plexyeventhandler.h \
    iconprovider.h \
    iconjob.h \
    icon.h \
    desktopbaseui.h

unix:!macx {
    SOURCES += plexywindow.cpp

    HEADERS += plexywindow.h \
            XAtoms.h
}

LIBS += -L./../build/lib -lplexymime -lplexyshaders -lplexydeskcore -lplexydeskuicore

TARGET = plexydesk

