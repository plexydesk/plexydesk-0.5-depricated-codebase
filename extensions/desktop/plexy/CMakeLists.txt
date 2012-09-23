SET(sourceFiles
    desktopview.cpp
    plexydesktopview.cpp
    desktopviewiface.cpp
    desktopviewimpl.cpp
    plexytray.cpp
    fileiconwidget.cpp
    plexygraphicsscene.cpp
    )

SET(headerFiles
    desktopview.h
    plexydesktopview.h
    desktopviewiface.h
    desktopviewimpl.h
    plexytray.h
    fileiconwidget.h
    plexygraphicsscene.h
    )

SET(QTMOC_SRCS
    desktopview.h
    plexydesktopview.h
    desktopviewimpl.h
    desktopviewiface.h
    plexytray.h
    fileiconwidget.h
    plexygraphicsscene.h
    )

QT4_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )

SET(libs
    ${QT_QTGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    )

ADD_LIBRARY(plexydesktopview SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(plexydesktopview PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(plexydesktopview
    ${PLEXY_UI_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS plexydesktopview DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES plexydesktopview.desktop DESTINATION share/plexy/ext/groups)
