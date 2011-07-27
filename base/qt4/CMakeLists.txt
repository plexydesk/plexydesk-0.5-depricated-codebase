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
    imagecache.cpp
    netwm.cpp
    pendingjob.cpp
    plexyconfig.cpp
    pluginloader.cpp
    viewlayer.cpp
    themepackloader.cpp 
    shaders/shadereffect.cpp
    shaders/shadereffectbuffer.cpp
    shaders/shadereffectitem.cpp
    shaders/shadereffectsource.cpp
    shaders/scenegraph/qsggeometry.cpp

    )

SET(headerFiles
   plexy.h
   debug.h
   abstractplugininterface.h
   backdropinterface.h
   backdropplugin.h
   baseplugin.h
   baserender.h
   datainterface.h
   dataplugin.h
   desktopwidget.h
   extensionfactory.h
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
   themepackloader.h
   shaders/shadereffect.h
   shaders/shadereffectbuffer.h
   shaders/shadereffectitem.h
   shaders/shadereffectsource.h
   shaders/scenegraph/qsggeometry.h

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
   imagecache.h
   pendingjob.h
   plexyconfig.h
   pluginloader.h
   socialinterface.h
   socialplugin.h
   viewlayer.h
   widgetinterface.h
   widgetplugin.h
   themepackloader.h
   shaders/shadereffect.h
   shaders/shadereffectbuffer.h
   shaders/shadereffectitem.h
   shaders/shadereffectsource.h
   shaders/scenegraph/qsggeometry.h
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
    ${QT_QTSVG_LIBRARY}
    ${DBUS_LIB}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTDECLARATIVE_LIBRARY}
    )

ADD_LIBRARY(${PLEXY_CORE_LIBRARY} SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(${PLEXY_CORE_LIBRARY} PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS ${PLEXY_CORE_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR})
