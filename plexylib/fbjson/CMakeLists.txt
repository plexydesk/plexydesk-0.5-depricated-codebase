# Check if we use any Debug in the final release and if so compile the tests
IF(CMAKE_BUILD_TYPE MATCHES ".*Deb.*")
    ADD_SUBDIRECTORY(test)
ENDIF(CMAKE_BUILD_TYPE MATCHES ".*Deb.*")


INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    )

SET(sourceFiles
    jsonhandler.cpp
    )

SET(headerFiles
    json_global.h
    )

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )

ADD_LIBRARY(plexyjson SHARED ${sourceFiles})

IF(MINGW)
    SET_TARGET_PROPERTIES(plexyjson PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(
    plexyjson
    ${QT_QTSCRIPT_LIBRARY}
    ${QT_QTWEBKIT_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTSVG_LIBRARY}
    )

INSTALL(TARGETS plexyjson DESTINATION lib)
