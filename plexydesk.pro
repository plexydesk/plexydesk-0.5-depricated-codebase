QTDIR = $$(QTDIR)

isEmpty(QTDIR) {
    error("You must set QTDIR before building project")
}

TEMPLATE = subdirs

ApplicationName = "PlexyDesktop"
ApplicationMainVersion = "0.6.0"
ApplicationDate = "2011-08-03"
ResourcesDir = "share/plexy/themepack/default/resources"

SUBDIRS += base 3rdparty

CONFIG += ordered
SUBDIRS += runner extensions modules

top_srcdir  = .\

! include( $${top_srcdir}/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

BUILD_DIR = $${top_outdir}
CONFIG_H = $${BUILD_DIR}/config.h

unix {
#    QMAKE_CXXFLAGS += -fPIC -fvisibility=default -fvisibility-inlines-hidden
#    QMAKE_LFLAGS += -shared -rdynamic

    system(mkdir $${BUILD_DIR})
    system(rm $${CONFIG_H})
    system(touch $${CONFIG_H})
}

win32 {
    BUILD_DIR = $$replace(BUILD_DIR,/,\\)
    CONFIG_H = $$replace(CONFIG_H,/,\\)
    !exists($${CONFIG_H}) {
        system(md $${BUILD_DIR})
    }
    system(del $${CONFIG_H})
    system(copy NUL $${CONFIG_H})
}

system(echo $${LITERAL_HASH}define PLEXYNAME \"$${ApplicationName}\" >> $${CONFIG_H})
system(echo $${LITERAL_HASH}define PLEXYVERSION \"$${ApplicationMainVersion}\" >> $${CONFIG_H})
system(echo $${LITERAL_HASH}define PLEXYDATE \"$${ApplicationDate}\" >> $${CONFIG_H})
system(echo $${LITERAL_HASH}define PLEXYQTIMPORTSDIR \"qt4//imports//\" >> $${CONFIG_H})
system(echo $${LITERAL_HASH}define PLEXYRESOURCESDIR \"$${ResourcesDir}\" >> $${CONFIG_H})

CONFIG(debug, debug|release) {
    system(echo $${LITERAL_HASH}define BUILD_MODE \"Debug\" >> $${CONFIG_H})
}
else {
    system(echo $${LITERAL_HASH}define BUILD_MODE \"Release\" >> $${CONFIG_H})
}

win32 {
    copyenv = xcopy /Y /F /E
}

unix {
    copyenv = cp -R
}

freedesktop.org.xml = $${IN_PWD}/3rdparty/qplexymime/freedesktop.org.xml $${sharedir}/plexy/mime/
mimetypes = $${IN_PWD}/3rdparty/qplexymime/mimetypes $${sharedir}/plexy/mime/
themepack.backdrop.qml = $${IN_PWD}/artwork/themepacks/default/backdrop/*.qml $${sharedir}/plexy/themepack/default/backdrop/
themepack.digitalclock.qml = $${IN_PWD}/artwork/themepacks/default/digitalclock/digitalClock.qml $${sharedir}/plexy/themepack/default/digitalclock/
themepack.digitalclock.images = $${IN_PWD}/artwork/themepacks/default/digitalclock/images/* $${sharedir}/plexy/themepack/default/resources/
themepack.folderview.qml = $${IN_PWD}/artwork/themepacks/default/folderview/folderview.qml $${sharedir}/plexy/themepack/default/folderview/
themepack.folderview.res.qml = $${IN_PWD}/artwork/themepacks/default/folderview/resources/Icon.qml $${sharedir}/plexy/themepack/default/folderview/resources/
themepack.folderview.res.js = $${IN_PWD}/artwork/themepacks/default/folderview/resources/Scripts.js $${sharedir}/plexy/themepack/default/folderview/resources/
themepack.folderview.res.images = $${IN_PWD}/artwork/themepacks/default/folderview/resources/images/* $${sharedir}/plexy/themepack/default/resources/
themepack.photo.qml = $${IN_PWD}/artwork/themepacks/default/photo/photoElement.qml $${sharedir}/plexy/themepack/default/photo/
themepack.photo.images = $${IN_PWD}/artwork/themepacks/default/photo/photoElement_images/* $${sharedir}/plexy/themepack/default/resources/
themepack.photoframe.qml = $${IN_PWD}/artwork/themepacks/default/photoframe/photoframe.qml $${sharedir}/plexy/themepack/default/photoframe/
themepack.photoframe.images = $${IN_PWD}/artwork/themepacks/default/photoframe/frameimages/* $${sharedir}/plexy/themepack/default/resources/
themepack.photoviewer.qml = $${IN_PWD}/artwork/themepacks/default/photoviewer/* $${sharedir}/plexy/themepack/default/photoviewer/
themepack.photoviewer.core.qml = $${IN_PWD}/artwork/themepacks/default/photoviewer/PhotoViewerCore/*.qml $${sharedir}/plexy/themepack/default/photoviewer/PhotoViewerCore/
themepack.photoviewer.core.scripts = $${IN_PWD}/artwork/themepacks/default/photoviewer/PhotoViewerCore/script/script.js $${sharedir}/plexy/themepack/default/photoviewer/PhotoViewerCore/script/
themepack.photoviewer.core.images = $${IN_PWD}/artwork/themepacks/default/photoviewer/PhotoViewerCore/images/* $${sharedir}/plexy/themepack/default/resources/
themepack.resources = $${IN_PWD}/artwork/themepacks/default/resources/* $${sharedir}/plexy/themepack/default/resources/
themepack.weather.qml = $${IN_PWD}/artwork/themepacks/default/weather/weather.qml $${sharedir}/plexy/themepack/default/weather/
themepack.welcome.qml = $${IN_PWD}/artwork/themepacks/default/welcome/welcome.qml $${sharedir}/plexy/themepack/default/welcome/
themepack.welcome.scripts = $${IN_PWD}/artwork/themepacks/default/welcome/scripts/Scripts.js $${sharedir}/plexy/themepack/default/welcome/scripts/
themepack.welcome.images = $${IN_PWD}/artwork/themepacks/default/welcome/images/* $${sharedir}/plexy/themepack/default/resources/
themepack.main = $${IN_PWD}/artwork/themepacks/default/main.cfg $${sharedir}/plexy/themepack/default/
plexydesk.icon = $${IN_PWD}/artwork/icons/plexydesk.png $${sharedir}/plexy/
artwork.widgets = $${IN_PWD}/artwork/widgets/* $${sharedir}/plexy/skins/widgets/

win32 {
    freedesktop.org.xml = $$replace(freedesktop.org.xml,/,\\)
    mimetypes = $$replace(mimetypes,/,\\)
    themepack.backdrop.qml = $$replace(themepack.backdrop.qml,/,\\)
    themepack.digitalclock.qml = $$replace(themepack.digitalclock.qml,/,\\)
    themepack.digitalclock.images = $$replace(themepack.digitalclock.images,/,\\)
    themepack.folderview.qml = $$replace(themepack.folderview.qml,/,\\)
    themepack.folderview.res.qml = $$replace(themepack.folderview.res.qml,/,\\)
    themepack.folderview.res.js = $$replace(themepack.folderview.res.js,/,\\)
    themepack.folderview.res.images = $$replace(themepack.folderview.res.images,/,\\)
    themepack.photo.qml = $$replace(themepack.photo.qml,/,\\)
    themepack.photo.images = $$replace(themepack.photo.images,/,\\)
    themepack.photoframe.qml = $$replace(themepack.photoframe.qml,/,\\)
    themepack.photoframe.images = $$replace(themepack.photoframe.images,/,\\)
    themepack.photoviewer.qml = $$replace(themepack.photoviewer.qml,/,\\)
    themepack.photoviewer.core.qml = $$replace(themepack.photoviewer.core.qml,/,\\)
    themepack.photoviewer.core.scripts = $$replace(themepack.photoviewer.core.scripts,/,\\)
    themepack.photoviewer.core.images = $$replace(themepack.photoviewer.core.images,/,\\)
    themepack.resources = $$replace(themepack.resources,/,\\)
    themepack.weather.qml = $$replace(themepack.weather.qml,/,\\)
    themepack.welcome.qml = $$replace(themepack.welcome.qml,/,\\)
    themepack.welcome.scripts = $$replace(themepack.welcome.scripts,/,\\)
    themepack.welcome.images = $$replace(themepack.welcome.images,/,\\)
    themepack.main = $$replace(themepack.main,/,\\)
    plexydesk.icon = $$replace(plexydesk.icon,/,\\)
    artwork.widgets = $$replace(artwork.widgets,/,\\)
}

system($${copyenv} $${freedesktop.org.xml})
system($${copyenv} $${mimetypes})
system($${copyenv} $${themepack.backdrop.qml})
system($${copyenv} $${themepack.digitalclock.qml})
system($${copyenv} $${themepack.digitalclock.images})
system($${copyenv} $${themepack.folderview.qml})
system($${copyenv} $${themepack.folderview.res.qml})
system($${copyenv} $${themepack.folderview.res.js})
system($${copyenv} $${themepack.folderview.res.images})
system($${copyenv} $${themepack.photo.qml})
system($${copyenv} $${themepack.photo.images})
system($${copyenv} $${themepack.photoframe.qml})
system($${copyenv} $${themepack.photoframe.images})
system($${copyenv} $${themepack.photoviewer.qml})
system($${copyenv} $${themepack.photoviewer.core.qml})
system($${copyenv} $${themepack.photoviewer.core.scripts})
system($${copyenv} $${themepack.photoviewer.core.images})
system($${copyenv} $${themepack.resources})
system($${copyenv} $${themepack.weather.qml})
system($${copyenv} $${themepack.welcome.qml})
system($${copyenv} $${themepack.welcome.scripts})
system($${copyenv} $${themepack.welcome.images})
system($${copyenv} $${themepack.main})
system($${copyenv} $${plexydesk.icon})
system($${copyenv} $${artwork.widgets})

win32 {
    QT_COPY_TO   = $${top_destdirbin}/
    QT_COPY_TO   = $$replace(QT_COPY_TO,/,\\)
    QT_COPY_FROM = $${QTDIR}\\bin

    CONFIG(debug, debug|release) {
        system($${copyenv} $${QT_COPY_FROM}\\QtCored4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtGuid4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtMultimediad4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtNetworkd4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtScriptd4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtWebKitd4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtXmld4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtDeclaratived4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtOpenGLd4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtScriptd4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtScriptToolsd4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtSqld4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtSvgd4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtXmlPatternsd4.dll $${QT_COPY_TO})
    }

    CONFIG(release, debug|release) {
        system($${copyenv} $${QT_COPY_FROM}\\QtCore4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtGui4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtMultimedia4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtNetwork4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtScript4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtWebKit4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtXml4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtDeclarative4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtOpenGL4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtScript4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtScriptTools4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtSql4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtSvg4.dll $${QT_COPY_TO})
        system($${copyenv} $${QT_COPY_FROM}\\QtXmlPatterns4.dll $${QT_COPY_TO})
    }
}

CONFIG(release, debug|release) {
    build_postfix = release
}

CONFIG(debug, debug|release) {
    build_postfix = debug
}

qtconf.path = $${OUT_PWD}/build/plexydesk-ready-build/bin
qtconf.file = $${qtconf.path}/qt.conf

unix {
    system(rm $${qtconf.file})
    system(touch $${qtconf.file})
}

win32 {
    qtconf.path = $$replace(qtconf.path,/,\\)
    qtconf.file = $$replace(qtconf.file,/,\\)

    !exists($${qtconf.file}) {
        system(md $${qtconf.path})
    }
    system(del $${qtconf.file})
    system(copy NUL $${qtconf.file})
}

system(echo [Paths] >> $${qtconf.file})
system(echo Prefix = ../ >> $${qtconf.file})
system(echo Plugins = lib >> $${qtconf.file})

# imports
imports.directory = $$[QT_INSTALL_IMPORTS]/*
imports.to = $${qtimports}
win32 {
    imports.directory = $$replace(imports.directory,/,\\)
    imports.to = $$replace(imports.to,/,\\)
}

system($${copyenv} $${imports.directory} $${imports.to})
