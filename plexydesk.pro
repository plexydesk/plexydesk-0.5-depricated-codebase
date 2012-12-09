QTDIR = $$(QTDIR)

isEmpty(QTDIR) {
    error("You must set QTDIR before building project")
}

TEMPLATE = subdirs

ApplicationName = "PlexyDesktop"
ApplicationMainVersion = "0.6.0"
ApplicationDate = "2011-08-03"
ResourcesDir = "share/plexy/themepack/default/resources"

CONFIG += ordered autogen_precompile_source warn_on debug

SUBDIRS += base 3rdparty runner extensions modules artwork

BUILD_DIR = $${OUT_PWD}/build/
CONFIG_H = $${OUT_PWD}/config.h

unix {
#    QMAKE_CXXFLAGS += -fPIC -fvisibility=default -fvisibility-inlines-hidden
#    QMAKE_LFLAGS += -shared -rdynamic

    !exists($${BUILD_DIR}) {
        system(md $${BUILD_DIR})
    }
    exists($${CONFIG_H}) {
        system(rm $${CONFIG_H})
    }
    system(touch $${CONFIG_H})
}

win32 {
    BUILD_DIR = $$replace(BUILD_DIR,/,\\)
    CONFIG_H = $$replace(CONFIG_H,/,\\)
    !exists($${BUILD_DIR}) {
        system(md $${BUILD_DIR})
    }
    exists($${CONFIG_H}) {
        system(del $${CONFIG_H})
    }
    system(copy NUL $${CONFIG_H})
}

win32 {
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
}

unix {
    system(echo \'$${LITERAL_HASH}define PLEXYNAME \"$${ApplicationName}\"\' >> $${CONFIG_H})
    system(echo \'$${LITERAL_HASH}define PLEXYVERSION \"$${ApplicationMainVersion}\"\' >> $${CONFIG_H})
    system(echo \'$${LITERAL_HASH}define PLEXYDATE \"$${ApplicationDate}\"\' >> $${CONFIG_H})
    system(echo \'$${LITERAL_HASH}define PLEXYQTIMPORTSDIR \"qt4//imports//\"\' >> $${CONFIG_H})
    system(echo \'$${LITERAL_HASH}define PLEXYRESOURCESDIR \"$${ResourcesDir}\"\' >> $${CONFIG_H})

    CONFIG(debug, debug|release) {
        system(echo \'$${LITERAL_HASH}define BUILD_MODE \"Debug\"\' >> $${CONFIG_H})
    }
    else {
        system(echo \'$${LITERAL_HASH}define BUILD_MODE \"Release\"\' >> $${CONFIG_H})
    }
}

win32 {
    copyenv = xcopy /Y /F /E
}

unix {
    copyenv = cp -R
}

win32 {
    QT_COPY_TO   = $${OUT_PWD}/build/bin/
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

unix {
    system(mkdir -p $${OUT_PWD}/build/qt4/imports)
}

qtconf.path = $${OUT_PWD}/build/bin
qtconf.file = $${qtconf.path}/qt.conf

unix {
    !exists($${qtconf.path}) {
        system(md $${qtconf.path})
    }
    exists($${qtconf.file}) {
        system(rm $${qtconf.file})
    }
    system(touch $${qtconf.file})
}

win32 {
    qtconf.path = $$replace(qtconf.path,/,\\)
    qtconf.file = $$replace(qtconf.file,/,\\)

    !exists($${qtconf.path}) {
        system(md $${qtconf.path})
    }
    exists($${qtconf.file}) {
        system(del $${qtconf.file})
    }
    system(copy NUL $${qtconf.file})
}

system(echo [Paths] >> $${qtconf.file})
system(echo Prefix = ../ >> $${qtconf.file})
system(echo Plugins = lib >> $${qtconf.file})

# imports
imports.directory = $$[QT_INSTALL_IMPORTS]/*
imports.to = $${OUT_PWD}/build/qt4/imports
win32 {
    imports.directory = $$replace(imports.directory,/,\\)
    imports.to = $$replace(imports.to,/,\\)
    system(mkdir $${imports.to})
}

system($${copyenv} $${imports.directory} $${imports.to})

