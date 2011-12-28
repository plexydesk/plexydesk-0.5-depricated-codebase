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
SUBDIRS += runner extensions modules artwork

top_srcdir  = .\

! include( $${top_srcdir}/common.pri ) {
    error( Couldn\'t find the common.pri file! )
}

BUILD_DIR = $${top_outdir}
CONFIG_H = $${BUILD_DIR}/config.h

unix {
#    QMAKE_CXXFLAGS += -fPIC -fvisibility=default -fvisibility-inlines-hidden
#    QMAKE_LFLAGS += -shared -rdynamic

    !exists($${CONFIG_H}) {
        system(mkdir -p $${BUILD_DIR})
    }
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

win32 {
    system(echo $${LITERAL_HASH}define PLEXYNAME \"$${ApplicationName}\" >> $${CONFIG_H})
    system(echo $${LITERAL_HASH}define PLEXYVERSION \"$${ApplicationMainVersion}\" >> $${CONFIG_H})
    system(echo $${LITERAL_HASH}define PLEXYDATE \"$${ApplicationDate}\" >> $${CONFIG_H})
    system(echo $${LITERAL_HASH}define PLEXYQTIMPORTSDIR \"qt4//imports//\" >> $${CONFIG_H})
    system(echo $${LITERAL_HASH}define PLEXYRESOURCESDIR \"$${ResourcesDir}\" >> $${CONFIG_H})
}

unix {
    system(echo \'$${LITERAL_HASH}define PLEXYNAME \"$${ApplicationName}\"\' >> $${CONFIG_H})
    system(echo \'$${LITERAL_HASH}define PLEXYVERSION \"$${ApplicationMainVersion}\"\' >> $${CONFIG_H})
    system(echo \'$${LITERAL_HASH}define PLEXYDATE \"$${ApplicationDate}\"\' >> $${CONFIG_H})
    system(echo \'$${LITERAL_HASH}define PLEXYQTIMPORTSDIR \"qt4//imports//\"\' >> $${CONFIG_H})
    system(echo \'$${LITERAL_HASH}define PLEXYRESOURCESDIR \"$${ResourcesDir}\"\' >> $${CONFIG_H})
}

CONFIG(debug, debug|release) {
    system(echo \'$${LITERAL_HASH}define BUILD_MODE \"Debug\"\' >> $${CONFIG_H})
}
else {
    system(echo \'$${LITERAL_HASH}define BUILD_MODE \"Release\"\' >> $${CONFIG_H})
}

win32 {
    copyenv = xcopy /Y /F /E
}

unix {
    copyenv = cp -R
}

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

unix {
    system(mkdir -p $${OUT_PWD}/build/plexydesk-ready-build/qt4/imports)
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
    system(mkdir $${imports.to})
}

system($${copyenv} $${imports.directory} $${imports.to})

