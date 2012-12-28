ADD_SUBDIRECTORY(clock)

SET(clocksourceFiles
    clock.cpp
    clockwidget.cpp
    clockinterface.cpp
    )

SET(headerFiles
    clock.h
    clockwidget.h
    clockinterface.h
    )

SET(QTMOC_SRCS
    clock.h
    clockwidget.h
    clockinterface.h
    )

QT5_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${clocksourceFiles}
    ${QT_MOC_SRCS}
    )

SET(libs
    ${PLEXY_UI_CORE_LIBRARY}
    )

ADD_LIBRARY(plexyclock SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(plexyclock PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(plexyclock
    ${libs}
    )
QT5_USE_MODULES(plexyclock Widgets Xml OpenGL Core WebKitWidgets Svg Declarative)

INSTALL(TARGETS plexyclock DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES clock.desktop DESTINATION share/plexy/ext/groups)
