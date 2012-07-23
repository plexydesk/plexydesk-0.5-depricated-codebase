SET(sourceFiles
    mac_background.cpp
    macbginterface.cpp
    )

SET(headerFiles
    mac_background.h
    macbginterface.h
    )

SET(QTMOC_SRCS
    mac_background.h
    macbginterface.h
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

ADD_LIBRARY(mac_background SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(mac_background PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(mac_background
    ${PLEXY_UI_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS mac_background DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES macdesktop.desktop DESTINATION share/plexy/ext/groups)
