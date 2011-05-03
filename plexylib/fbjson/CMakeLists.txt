INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
)

SET(sourceFiles
    jsonhandler.cpp
)

SET(headerFiles
    json_global.h
    jsonhandler.cpp
)

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
)

ADD_LIBRARY (plexyjson SHARED ${sourceFiles})

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

IF(WIN32)
    INSTALL(TARGETS plexyjson DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
ELSEIF(WIN32)
    INSTALL(TARGETS plexyjson DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
ENDIF(WIN32)

ADD_SUBDIRECTORY(test)
