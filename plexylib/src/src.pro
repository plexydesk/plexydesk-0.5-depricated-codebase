TEMPLATE = lib
TARGET = qmlshadersplugin
QT += declarative opengl
CONFIG += qt plugin

TARGETPATH = Qt/labs/shaders
target.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

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
    shadereffectbuffer.h

OTHER_FILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmlfiles.files = qmldir
qmlfiles.path = $$[QT_INSTALL_IMPORTS]/$$TARGETPATH

INSTALLS += target qmlfiles
