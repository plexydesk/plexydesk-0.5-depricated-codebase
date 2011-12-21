top_srcdir  = ../../..
srcdir = plexyshaders

! include( $${top_srcdir}/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

INCLUDEPATH += $${IN_PWD}/../
TEMPLATE = lib
TARGET = plexyshaders
QT += declarative opengl
CONFIG += qt plugin

DEFINES += plexyshaders_EXPORTS

win32:!wince*:DLLDESTDIR = $${top_destdirbin}

symbian: {
    TARGET.EPOCALLOWDLLDATA = 1

    pluginstub.sources = $${TARGET}.dll
    pluginstub.path = /resource/qt/imports/$$TARGETPATH

    resfiles.sources = qmldir
    resfiles.path = /resource/qt/imports/$$TARGETPATH

    DEPLOYMENT += pluginstub resfiles

# temp workaround
    TARGET.CAPABILITY += NetworkServices
}

# Input
SOURCES += \
    qmlshadersplugin_plugin.cpp \
    shadereffect.cpp \
    shadereffectitem.cpp \
    shadereffectsource.cpp \
    scenegraph/qsggeometry.cpp \
    shadereffectbuffer.cpp

HEADERS += \
    qmlshadersplugin_plugin.h \
    glfunctions.h \
    shadereffect.h \
    shadereffectitem.h \
    shadereffectsource.h \
    scenegraph/qsggeometry.h \
    shadereffectbuffer.h \
    shaders_global.h

OTHER_FILES = qmldir

!equals(_PRO_FILE_PWD_, $${DESTDIR}) {
    copy_qmldir.target = $${DESTDIR}/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

TARGETPATH = Qt/labs/shaders

qmlfiles.files = qmldir
qmlfiles.path = $${qtimports}/$$TARGETPATH

INSTALLS += qmlfiles

target.path = $${qtimports}/$$TARGETPATH
INSTALLS += target
