SET (sourceFiles
    bbconn.cpp
    bbconninterface.cpp
    server.cpp
    connection.cpp
    peermanager.cpp
    client.cpp
    sslserver.cpp
    sslclient.cpp
    )

SET(headerFiles
    bbconn.h
    bbconninterface.h
    server.h
    connection.h
    peermanager.h
    client.h
    sslserver.h
    sslclient.h
    )

SET (QTMOC_SRCS
    bbconn.h
    bbconninterface.h
    server.h
    connection.h
    peermanager.h
    client.h
    sslserver.h
    sslclient.h
    )

QT4_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )

SET(libs
    ${QT_QTCORE_LIBRARY}
    )

ADD_LIBRARY(bbconnengine SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(bbconnengine PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(bbconnengine
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS bbconnengine DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES bbconn.desktop DESTINATION share/plexy/ext/groups)
