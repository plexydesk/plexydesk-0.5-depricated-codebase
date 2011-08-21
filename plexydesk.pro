TEMPLATE = subdirs

ApplicationName = "PlexyDesktop"
ApplicationMainVersion = "0.6.0"
ApplicationDate = "2011-08-03"
ResourcesDir = "share/plexy/themepack/default/resources"

unix {
    QMAKE_CXXFLAGS += -fPIC -fvisibility=default -fvisibility-inlines-hidden
    QMAKE_LFLAGS += -shared -rdynamic

    system(rm build/config.h)
    system(echo $${LITERAL_HASH}define PLEXYNAME $${ApplicationName} >> build/config.h)
}

win32 {
    QMAKE_CXXFLAGS += -fPIC -fvisibility=default -fvisibility-inlines-hidden
    QMAKE_LFLAGS += -shared -rdynamic

    system(del build\\config.h)
    system(copy NUL build\\config.h)
    system(echo $${LITERAL_HASH}define PLEXYNAME \"$${ApplicationName}\" >> build\\config.h)
    system(echo $${LITERAL_HASH}define PLEXYVERSION \"$${ApplicationMainVersion}\" >> build\\config.h)
    system(echo $${LITERAL_HASH}define PLEXYDATE \"$${ApplicationDate}\" >> build\\config.h)
    system(echo $${LITERAL_HASH}define PLEXYQTIMPORTSDIR \"qt4\\imports\" >> build\\config.h)
    system(echo $${LITERAL_HASH}define PLEXYRESOURCESDIR \"$${ResourcesDir}\" >> build\\config.h)

    CONFIG(debug, debug|release) {
        system(echo $${LITERAL_HASH}define BUILD_MODE \"Debug\" >> build\\config.h)
    }
    else {
        system(echo $${LITERAL_HASH}define BUILD_MODE \"Release\" >> build\\config.h)
    }
}

SUBDIRS = base

CONFIG += ordered
#SUBDIRS += runner
