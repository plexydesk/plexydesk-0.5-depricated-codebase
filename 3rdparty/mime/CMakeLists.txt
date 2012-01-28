# Check if we use any Debug in the final release and if so compile the tests
IF(CMAKE_BUILD_TYPE MATCHES ".*Deb.*")
    ADD_SUBDIRECTORY(test)
ENDIF(CMAKE_BUILD_TYPE MATCHES ".*Deb.*")


SET (sourceFiles
    freedesktopmime.cpp
    )

SET(headerFiles
    freedesktopmime.h
    mime_globals.h
    )

SET(MOC_SRCS
    freedesktopmime.h
    )

QT4_WRAP_CPP(QT_MOC_SRCS ${MOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )

SET(libs
    ${QT_QTGUI_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTSVG_LIBRARY}
    )

ADD_LIBRARY (mimetype SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(mimetype PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(mimetype
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS mimetype DESTINATION ${CMAKE_INSTALL_LIBDIR})
