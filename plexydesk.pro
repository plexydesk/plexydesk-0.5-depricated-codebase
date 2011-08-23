TEMPLATE = subdirs

ApplicationName = "PlexyDesktop"
ApplicationMainVersion = "0.6.0"
ApplicationDate = "2011-08-03"
ResourcesDir = "share/plexy/themepack/default/resources"

SUBDIRS = base 3rdparty

CONFIG += ordered
SUBDIRS += runner extensions

unix {
#    QMAKE_CXXFLAGS += -fPIC -fvisibility=default -fvisibility-inlines-hidden
#    QMAKE_LFLAGS += -shared -rdynamic

    system(rm build\\config.h)
    system(touch build\\config.h)
}

win32 {
    system(del build\\config.h)
    system(copy NUL build\\config.h)
}

system(echo $${LITERAL_HASH}define PLEXYNAME \"$${ApplicationName}\" >> build\\config.h)
system(echo $${LITERAL_HASH}define PLEXYVERSION \"$${ApplicationMainVersion}\" >> build\\config.h)
system(echo $${LITERAL_HASH}define PLEXYDATE \"$${ApplicationDate}\" >> build\\config.h)
system(echo $${LITERAL_HASH}define PLEXYQTIMPORTSDIR \"qt4//imports//\" >> build\\config.h)
system(echo $${LITERAL_HASH}define PLEXYRESOURCESDIR \"$${ResourcesDir}\" >> build\\config.h)

CONFIG(debug, debug|release) {
    system(echo $${LITERAL_HASH}define BUILD_MODE \"Debug\" >> build\\config.h)
}
else {
    system(echo $${LITERAL_HASH}define BUILD_MODE \"Release\" >> build\\config.h)
}

top_srcdir  = .\

! include( $$top_srcdir/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

win32 {
    copyenv = xcopy /Y /F /E
}

unix {
    copyenv = cp -R
}

system($$copyenv 3rdparty\\qplexymime\\freedesktop.org.xml $${sharedir}\\plexy\\mime\\)
system($$copyenv 3rdparty\\qplexymime\\mimetypes $${sharedir}\\plexy\\mime\\)
system($$copyenv artwork\\themepacks\\default\\backdrop\\* $${sharedir}\\plexy\\themepack\\default\\backdrop\\)
system($$copyenv artwork\\themepacks\\default\\digitalclock\\* $${sharedir}\\plexy\\themepack\\default\\digitalclock\\)
system($$copyenv artwork\\themepacks\\default\\folderview\\* $${sharedir}\\plexy\\themepack\\default\\folderview\\)
system($$copyenv artwork\\themepacks\\default\\photo\\* $${sharedir}\\plexy\\themepack\\default\\photo\\)
system($$copyenv artwork\\themepacks\\default\\photoframe\\* $${sharedir}\\plexy\\themepack\\default\\photoframe\\)
system($$copyenv artwork\\themepacks\\default\\photoviewer\\* $${sharedir}\\plexy\\themepack\\default\\photoviewer\\)
system($$copyenv artwork\\themepacks\\default\\resources\\* $${sharedir}\\plexy\\themepack\\default\\resources\\)
system($$copyenv artwork\\themepacks\\default\\weather\\* $${sharedir}\\plexy\\themepack\\default\\weather\\)
system($$copyenv artwork\\themepacks\\default\\welcome\\* $${sharedir}\\plexy\\themepack\\default\\welcome\\)
system($$copyenv artwork\\themepacks\\default\\main.cfg $${sharedir}\\plexy\\themepack\\default\\)
system($$copyenv artwork\\icons\\plexydesk.png $${sharedir}\\plexy\\)
system($$copyenv artwork\\widgets\\* $${sharedir}\\plexy\\skins\\widgets\\)

QT_COPY_TO   = $$replace(OUT_PWD,/,\\)\\$${DESTDIR}\\

win32 {
    CONFIG(debug, debug|release) {
        system($$copyenv %QTDIR%\\bin\\QtCored4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtGuid4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtMultimediad4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtNetworkd4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtScriptd4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtWebKitd4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtXmld4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtDeclaratived4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtOpenGLd4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtScriptd4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtScriptToolsd4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtSqld4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtSvgd4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtXmlPatternsd4.dll $${QT_COPY_TO})
    }

    CONFIG(release, debug|release) {
        system($$copyenv %QTDIR%\\bin\\QtCore4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtGui4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtMultimedia4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtNetwork4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtScript4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtWebKit4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtXml4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtDeclarative4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtOpenGL4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtScript4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtScriptTools4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtSql4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtSvg4.dll $${QT_COPY_TO})
        system($$copyenv %QTDIR%\\bin\\QtXmlPatterns4.dll $${QT_COPY_TO})
    }
}

CONFIG(release, debug|release) {
    build_postfix = release
}

CONFIG(debug, debug|release) {
    build_postfix = debug
}

qtconf = $${PWD}/build/$$build_postfix/lib/qt4

unix {
    system(rm \"$${top_destdir}\\qt.conf\")
    system(touch NUL \"$${top_destdir}\\qt.conf\")
}

win32 {
    system(del \"$${top_destdir}\\qt.conf\")
    system(copy NUL \"$${top_destdir}\\qt.conf\")
}

system(echo [Paths] >> \"$$top_destdir\\qt.conf\")
system(echo Prefix = $${qtconf} >> \"$$top_destdir\\qt.conf\")
