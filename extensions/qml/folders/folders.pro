top_srcdir  = ../../..
srcdir      = folders_qml

!include( $$top_srcdir/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

TEMPLATE = lib

QT += core declarative network opengl xml

DEFINES += folderview_EXPORTS

win32:!wince*:DLLDESTDIR = $${top_destdirbin}

INCLUDEPATH += $$top_srcdir/base/qt4 $$top_srcdir/base/shaders $$top_srcdir/3rdparty/mime

CONFIG += qt plugin

SOURCES = plugin.cpp \
        support.cpp

HEADERS = plugin.h \
        support.h

LIBS += -lplexyshaders -lplexydeskcore -lmimetype

TARGET = folderview

OTHER_FILES = qmldir

!equals(_PRO_FILE_PWD_, $${DESTDIR}) {
    copy_qmldir.target = $${DESTDIR}/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

TARGETPATH = FolderView

qmlfiles.files = qmldir
qmlfiles.path = $${qtimports}/$$TARGETPATH

INSTALLS += qmlfiles

target.path = $${qtimports}/$$TARGETPATH
INSTALLS += target

