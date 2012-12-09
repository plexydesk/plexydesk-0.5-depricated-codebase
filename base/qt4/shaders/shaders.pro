INCLUDEPATH += ../ ../../core

TEMPLATE = lib
TARGET = plexyshaders
QT += declarative opengl
CONFIG += qt plugin

DEFINES += plexyshaders_EXPORTS

DESTDIR += $${OUT_PWD}/../../../build/lib
win32:!wince*:DLLDESTDIR = $${OUT_PWD}/../../../build/bin

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

!equals(_PRO_FILE_PWD_, $${OUT_PWD}/../../../build/bin) {
    copy_qmldir.target = $${OUT_PWD}/../../../build/bin/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

TARGETPATH = Qt/labs/shaders

qmlfiles.files = qmldir
qmlfiles.path = $${OUT_PWD}/../../../build/qt4/imports/$$TARGETPATH

INSTALLS += qmlfiles

target.path = $${OUT_PWD}/../../../build/qt4/imports/$$TARGETPATH
INSTALLS += target
