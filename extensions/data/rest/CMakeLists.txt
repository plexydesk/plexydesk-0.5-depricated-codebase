INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    )

SET (sourceFiles
    rest.cpp
    restinterface.cpp
    )

SET(headerFiles
    rest.h
    restinterface.h
    )

SET (QTMOC_SRCS
    rest.h
    restinterface.h
    )

QT4_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )

SET(libs
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    )

ADD_LIBRARY(restengine SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(restengine PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(
    restengine
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS restengine DESTINATION lib/plexyext)
INSTALL(FILES rest.desktop DESTINATION share/plexy/ext/groups)
