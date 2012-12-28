SET (sourceFiles
    timer.cpp
    timerinterface.cpp
    )

SET(headerFiles
    timer.h
    timerinterface.h
    )

SET (QTMOC_SRCS
    timer.h
    timerinterface.h
    )

QT5_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )

ADD_LIBRARY(timerengine SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(timerengine PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(timerengine
    ${PLEXY_CORE_LIBRARY}
    )

QT5_USE_MODULES(timerengine Widgets Xml OpenGL Core WebKitWidgets Svg Declarative)

INSTALL(TARGETS timerengine DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES timer.desktop DESTINATION share/plexy/ext/groups)
