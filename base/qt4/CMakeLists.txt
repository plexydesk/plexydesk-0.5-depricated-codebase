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
   canvas.cpp
   baserender.cpp
   desktopwidget.cpp
   extensionfactory.cpp
   fakemime.cpp
   netwm.cpp
   plexyconfig.cpp
   pluginloader.cpp
   viewlayer.cpp
   pendingjob.cpp
   listview.cpp
   imagecache.cpp
   httpjobhandler.cpp
)

SET(headerFiles
   abstractplugininterface.h
   backdropinterface.h
   backdropplugin.h
   baseplugin.h
   baserender.h
   canvas.h
   datainterface.h
   dataplugin.h
   desktopwidget.h
   extensionfactory.h
   fakemime.h
   netwm.h
   netwm_def.h
   netwm_p.h
   plexyconfig.h
   pluginloader.h
   viewlayer.h
   widgetinterface.h
   widgetplugin.h
   pendingjob.h
   listview.h
   imagecache.h
   httpjobhandler.h
)

SET(QTMOC_SRCS
   abstractplugininterface.h
   backdropplugin.h
   baseplugin.h
   baserender.h
   canvas.h
   datainterface.h
   dataplugin.h
   desktopwidget.h
   plexyconfig.h
   pluginloader.h
   viewlayer.h
   widgetplugin.h
   widgetinterface.h
   pendingjob.h
   listview.h
   imagecache.h
   httpjobhandler.h
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

qt4_wrap_cpp(QT_MOC_SRCS ${QTMOC_SRCS})

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

TARGET_LINK_LIBRARIES(
    plexydeskcore
    plexymime
    ${libs}
)
IF (NOT WIN32)
    INSTALL(TARGETS plexydeskcore DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
ELSEIF(WIN32)
    INSTALL(TARGETS plexydeskcore DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
ENDIF(NOT WIN32)
