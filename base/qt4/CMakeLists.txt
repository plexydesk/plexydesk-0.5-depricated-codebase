SET(sourceFiles
    baserender.cpp
    extensionfactory.cpp
    netwm.cpp
    themepackloader.cpp
    viewlayer.cpp
    shaders/shadereffect.cpp
    shaders/shadereffectbuffer.cpp
    shaders/shadereffectitem.cpp
    shaders/shadereffectsource.cpp
    shaders/scenegraph/qsggeometry.cpp
    qmldesktopwidget.cpp
    desktopwidget.cpp
    scrollwidget.cpp
    plexyqmlglue.cpp
    imagecache.cpp
    svgprovider.cpp
    qmlsvgprovider.cpp
    qmlpixmapprovider.cpp
    plexyconfig.cpp
    style.cpp
    nativestyle.cpp
    button.cpp
    )

SET(headerFiles 
    baserender.h
    extensionfactory.h
    netwm.h
    netwm_def.h
    netwm_p.h
    socialinterface.h
    socialplugin.h
    themepackloader.h
    viewlayer.h
    shaders/shadereffect.h
    shaders/shadereffectbuffer.h
    shaders/shadereffectitem.h
    shaders/shadereffectsource.h
    shaders/scenegraph/qsggeometry.h
    qmldesktopwidget.h
    desktopwidget.h
    scrollwidget.h
    plexyqmlglue.h
    imagecache.h
    svgprovider.h
    qmlsvgprovider.h
    qmlpixmapprovider.h
    plexyconfig.h
    style.h
    nativestyle.h
    button.h
    )

SET(MOC_SRCS
   baserender.h
   socialinterface.h
   socialplugin.h
   themepackloader.h
   viewlayer.h
   shaders/shadereffect.h
   shaders/shadereffectitem.h
   shaders/shadereffectsource.h
   qmldesktopwidget.h
   desktopwidget.h
   scrollwidget.h
   imagecache.h
   svgprovider.h
   qmlsvgprovider.h
   qmlpixmapprovider.h
   plexyconfig.h
   nativestyle.h
   style.h
   button.h
   )

# DBUS is working only under linux for now
IF (NOT APPLE)
    IF (UNIX)
        QT4_ADD_DBUS_ADAPTOR(dbus_SRCS org.plexydesk.Config.xml
                             ${CMAKE_SOURCE_DIR}/base/qt4/plexyconfig.h PlexyDesk::Config)
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
    ${QT_QTXML_LIBRARY}
    ${PLEXY_CORE_LIBRARY}
    )

ADD_LIBRARY(${PLEXY_UI_CORE_LIBRARY} SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(${PLEXY_UI_CORE_LIBRARY} PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(${PLEXY_UI_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS ${PLEXY_UI_CORE_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR})
