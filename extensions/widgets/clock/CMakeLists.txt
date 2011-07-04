ADD_SUBDIRECTORY(clock)

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    )

SET(clocksourceFiles
    clock.cpp
    clockwidget.cpp
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

QT4_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${clocksourceFiles}
    ${QT_MOC_SRCS}
    )

SET(libs
    ${PLEXY_CORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    )

ADD_LIBRARY(plexyclock SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(plexyclock PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(
    plexyclock
    ${libs}
    )

INSTALL(TARGETS plexyclock DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext)
INSTALL(FILES clock.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/plexydesk/ext/groups)
