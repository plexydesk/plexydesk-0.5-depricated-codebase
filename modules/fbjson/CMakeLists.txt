# Check if we use any Debug in the final release and if so compile the tests
IF(CMAKE_BUILD_TYPE MATCHES ".*Deb.*")
    ADD_SUBDIRECTORY(test)
ENDIF(CMAKE_BUILD_TYPE MATCHES ".*Deb.*")


SET(sourceFiles
    jsonhandler.cpp
    facebooksession.cpp
    )

SET(headerFiles
    json_global.h
    facebooksession.h
    )

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )


QT4_WRAP_CPP(QT_MOC_SRCS ${headerFiles})

ADD_LIBRARY(plexyjson SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(plexyjson PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(plexyjson
    ${QT_QTSCRIPT_LIBRARY}
    ${QT_QTWEBKIT_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTSVG_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${PLEXY_UI_CORE_LIBRARY}
    )

INSTALL(TARGETS plexyjson DESTINATION ${CMAKE_INSTALL_LIBDIR})

ADD_SUBDIRECTORY (test) 
