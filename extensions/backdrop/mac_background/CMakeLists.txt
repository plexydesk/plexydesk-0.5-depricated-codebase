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

QT5_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
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
    )
QT5_USE_MODULES(mac_background Widgets Xml OpenGL Core WebKitWidgets Svg Declarative)

INSTALL(TARGETS mac_background DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES macdesktop.desktop DESTINATION share/plexy/ext/groups)
