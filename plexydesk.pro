QTDIR = $$(QTDIR)

isEmpty(QTDIR) {
    error("You must set QTDIR before building project")
}

TEMPLATE = subdirs

ApplicationName = "PlexyDesktop"
ApplicationMainVersion = "0.6.0"
ApplicationDate = "2011-08-03"
ResourcesDir = "share/plexy/themepack/default/resources"

SUBDIRS += base #3rdparty

CONFIG += ordered
#SUBDIRS += runner extensions

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
themepack.backdrop = $${IN_PWD}/artwork/themepacks/default/backdrop/* $${sharedir}/plexy/themepack/default/backdrop/
themepack.digitalclock = $${IN_PWD}/artwork/themepacks/default/digitalclock/* $${sharedir}/plexy/themepack/default/digitalclock/
themepack.folderview = $${IN_PWD}/artwork/themepacks/default/folderview/* $${sharedir}/plexy/themepack/default/folderview/
themepack.photo = $${IN_PWD}/artwork/themepacks/default/photo/* $${sharedir}/plexy/themepack/default/photo/
themepack.photoframe = $${IN_PWD}/artwork/themepacks/default/photoframe/* $${sharedir}/plexy/themepack/default/photoframe/
themepack.photoviewer = $${IN_PWD}/artwork/themepacks/default/photoviewer/* $${sharedir}/plexy/themepack/default/photoviewer/
themepack.resources = $${IN_PWD}/artwork/themepacks/default/resources/* $${sharedir}/plexy/themepack/default/resources/
themepack.weather = $${IN_PWD}/artwork/themepacks/default/weather/* $${sharedir}/plexy/themepack/default/weather/
themepack.welcome = $${IN_PWD}/artwork/themepacks/default/welcome/* $${sharedir}/plexy/themepack/default/welcome/
themepack.main = $${IN_PWD}/artwork/themepacks/default/main.cfg $${sharedir}/plexy/themepack/default/
plexydesk.icon = $${IN_PWD}/artwork/icons/plexydesk.png $${sharedir}/plexy/
artwork.widgets = $${IN_PWD}/artwork/widgets/* $${sharedir}/plexy/skins/widgets/

win32 {
    freedesktop.org.xml = $$replace(freedesktop.org.xml,/,\\)
    mimetypes = $$replace(mimetypes,/,\\)
    themepack.backdrop = $$replace(themepack.backdrop,/,\\)
    themepack.digitalclock = $$replace(themepack.digitalclock,/,\\)
    themepack.folderview = $$replace(themepack.folderview,/,\\)
    themepack.photo = $$replace(themepack.photo,/,\\)
    themepack.photoframe = $$replace(themepack.photoframe,/,\\)
    themepack.photoviewer = $$replace(themepack.photoviewer,/,\\)
    themepack.resources = $$replace(themepack.resources,/,\\)
    themepack.weather = $$replace(themepack.weather,/,\\)
    themepack.welcome = $$replace(themepack.welcome,/,\\)
    themepack.main = $$replace(themepack.main,/,\\)
    plexydesk.icon = $$replace(plexydesk.icon,/,\\)
    artwork.widgets = $$replace(artwork.widgets,/,\\)
}

system($${copyenv} $${freedesktop.org.xml})
system($${copyenv} $${mimetypes})
system($${copyenv} $${themepack.backdrop})
system($${copyenv} $${themepack.digitalclock})
system($${copyenv} $${themepack.folderview})
system($${copyenv} $${themepack.photo})
system($${copyenv} $${themepack.photoframe})
system($${copyenv} $${themepack.photoviewer})
system($${copyenv} $${themepack.resources})
system($${copyenv} $${themepack.weather})
system($${copyenv} $${themepack.welcome})
system($${copyenv} $${themepack.main})
system($${copyenv} $${plexydesk.icon})
system($${copyenv} $${artwork.widgets})

win32 {
    QT_COPY_TO   = $${top_destdirlib}/
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

qtconf.path = $${top_destdirbin}
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
system(echo Prefix = $${qtconf.path} >> $${qtconf.file})
