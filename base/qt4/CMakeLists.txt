SET(QT_USE_OPENGL TRUE)

INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
)

SET(sourceFiles
    abstractplugininterface.cpp
    backdropinterface.cpp
    backdropplugin.cpp
    baserender.cpp
    desktopwidget.cpp
    extensionfactory.cpp
    fakemime.cpp
    httpjobhandler.cpp
    imagecache.cpp
    netwm.cpp
    pendingjob.cpp
    plexyconfig.cpp
    pluginloader.cpp
    viewlayer.cpp
)

SET(headerFiles
   abstractplugininterface.h
   backdropinterface.h
   backdropplugin.h
   baseplugin.h
   baserender.h
   datainterface.h
   dataplugin.h
   desktopwidget.h
   extensionfactory.h
   fakemime.h
   httpjobhandler.h
   imagecache.h
   netwm.h
   netwm_def.h
   netwm_p.h
   pendingjob.h
   plexyconfig.h
   pluginloader.h
   socialinterface.h
   socialplugin.h
   viewlayer.h
   widgetinterface.h
   widgetplugin.h
)

SET(MOC_SRCS
   abstractplugininterface.h
   backdropinterface.h
   backdropplugin.h
   baseplugin.h
   baserender.h
   datainterface.h
   dataplugin.h
   desktopwidget.h
   httpjobhandler.h
   imagecache.h
   pendingjob.h
   plexyconfig.h
   pluginloader.h
   socialinterface.h
   socialplugin.h
   viewlayer.h
   widgetinterface.h
   widgetplugin.h
)

#for now only dbus works only under linux
IF (NOT APPLE)
IF (UNIX)
QT4_ADD_DBUS_ADAPTOR(dbus_SRCS org.plexydesk.Config.xml
                        plexyconfig.h PlexyDesk::Config)
SET (DBUS_LIB ${QT_QTDBUS_LIBRARY})
MESSAGE(${dbus_SRCS})
ENDIF(UNIX)

ENDIF (NOT APPLE)

qt4_wrap_cpp(QT_MOC_SRCS ${MOC_SRCS})

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
    ${QT_QTSVG_LIBRARY}
    ${DBUS_LIB}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTDECLARATIVE_LIBRARY}
)

ADD_LIBRARY(plexydeskcore SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(plexydeskcore PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
    )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(
    plexydeskcore
    ${libs}
)

IF (NOT WIN32)
    INSTALL(TARGETS plexydeskcore DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
ELSEIF(WIN32)
    INSTALL(TARGETS plexydeskcore DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
ENDIF(NOT WIN32)
