SET(sourceFiles
    abstractplugininterface.cpp
    abstractdesktopwidget.cpp
    abstractdesktopview.cpp
    backdropinterface.cpp
    backdropplugin.cpp
    baseplugin.h
    imagecache.cpp
    svgprovider.cpp
    plexyconfig.cpp
    pluginloader.cpp
    pendingjob.cpp
    )

SET(headerFiles
    abstractplugininterface.h
    abstractdesktopwidget.h
    backdropinterface.h
    backdropplugin.h
    baseplugin.h
    plexy.h
    debug.h
    plexyconfig.h
    datainterface.h
    dataplugin.h
    pluginloader.h
    abstractdesktopview.h
    pendingjob.h
    desktopviewplugin.h
   )

SET(MOC_SRCS
    abstractplugininterface.h
    abstractdesktopwidget.h
    backdropinterface.h
    backdropplugin.h
    baseplugin.h
    datainterface.h
    dataplugin.h
    plexyconfig.h
    imagecache.h
    svgprovider.h
    pluginloader.h
    abstractdesktopview.h
    pendingjob.h
    widgetinterface.h
    widgetplugin.h
    desktopviewplugin.h
   )

# DBUS is working only under linux for now
IF (NOT APPLE)
    IF (UNIX)
        QT4_ADD_DBUS_ADAPTOR(dbus_SRCS org.plexydesk.Config.xml
                             plexyconfig.h PlexyDesk::Config)
        SET (DBUS_LIB ${QT_QTDBUS_LIBRARY})
    ENDIF(UNIX)
ENDIF (NOT APPLE)

QT4_WRAP_CPP(QT_MOC_SRCS ${MOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    ${dbus_SRCS}
    )

SET(libs
    ${QT_QTGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    ${DBUS_LIB}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTSVG_LIBRARY}
    )

ADD_LIBRARY(${PLEXY_CORE_LIBRARY} SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(${PLEXY_CORE_LIBRARY} PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(${PLEXY_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS ${PLEXY_CORE_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR})
