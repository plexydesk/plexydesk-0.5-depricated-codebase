SET(sourceFiles
    abstractplugininterface.cpp
    backdropinterface.cpp
    backdropplugin.cpp
    baserender.cpp
    desktopwidget.cpp
    extensionfactory.cpp
    imagecache.cpp
    svgprovider.cpp
    netwm.cpp
    pendingjob.cpp
    plexyconfig.cpp
    pluginloader.cpp
    themepackloader.cpp
    viewlayer.cpp
    shaders/shadereffect.cpp
    shaders/shadereffectbuffer.cpp
    shaders/shadereffectitem.cpp
    shaders/shadereffectsource.cpp
    shaders/scenegraph/qsggeometry.cpp
    qmldesktopwidget.cpp
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
    svgprovider.h
    netwm.h
    netwm_def.h
    netwm_p.h
    pendingjob.h
    plexyconfig.h
    pluginloader.h
    socialinterface.h
    socialplugin.h
    themepackloader.h
    viewlayer.h
    widgetinterface.h
    widgetplugin.h
    shaders/shadereffect.h
    shaders/shadereffectbuffer.h
    shaders/shadereffectitem.h
    shaders/shadereffectsource.h
    shaders/scenegraph/qsggeometry.h
    qmldesktopwidget.h
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
   pendingjob.h
   plexyconfig.h
   pluginloader.h
   socialinterface.h
   socialplugin.h
   themepackloader.h
   viewlayer.h
   widgetinterface.h
   widgetplugin.h
   shaders/shadereffect.h
   shaders/shadereffectitem.h
   shaders/shadereffectsource.h
   qmldesktopwidget.h
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

TARGET_LINK_LIBRARIES(${PLEXY_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS ${PLEXY_CORE_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR})
