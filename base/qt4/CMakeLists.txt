SET(sourceFiles
    extensionfactory.cpp
    netwm.cpp
    themepackloader.cpp
    viewlayer.cpp
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
    lineedit.cpp
    webkitwebview.cpp
    texteditor.cpp
    windowbutton.cpp
    )

SET(headerFiles
    extensionfactory.h
    netwm.h
    netwm_def.h
    netwm_p.h
    themepackloader.h
    viewlayer.h
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
    lineedit.h
    webkitwebview.h
    texteditor.h
    windowbutton.h
    )

SET(MOC_SRCS
   themepackloader.h
   viewlayer.h
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
   lineedit.h
   webkitwebview.h
   texteditor.h
   windowbutton.h
   )

# DBUS is working only under linux for now
#IF (NOT APPLE)
#    IF (UNIX)
#        QT4_ADD_DBUS_ADAPTOR(dbus_SRCS org.plexydesk.Config.xml
#                             ${CMAKE_SOURCE_DIR}/base/qt4/plexyconfig.h PlexyDesk::Config)
#        SET (DBUS_LIB ${QT_QTDBUS_LIBRARY})
#    ENDIF(UNIX)
#ENDIF (NOT APPLE)

#Locate cococa framework on mac
IF (APPLE)
   INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
   FIND_LIBRARY(COCOA_LIBRARY Cocoa)
   FIND_LIBRARY(GLUT_LIBRARY GLUT)
   FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
   MARK_AS_ADVANCED (COCOA_LIBRARY
                     GLUT_LIBRARY
                     OpenGL_LIBRARY)
   SET(MAC_LIBS ${COCOA_LIBRARY} ${GLUT_LIBRARY} ${OpenGL_LIBRARY})
ENDIF(APPLE)

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
    ${QT_QTWEBKIT_LIBRARY}
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
    ${MAC_LIBS}
    )

INSTALL(TARGETS ${PLEXY_UI_CORE_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR})
