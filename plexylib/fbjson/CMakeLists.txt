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

ADD_LIBRARY (plexyjson SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
        plexyjson
        ${QT_QTSCRIPT_LIBRARY}
        ${QT_QTWEBKIT_LIBRARY}
        ${QT_QTGUI_LIBRARY}
        ${QT_QTCORE_LIBRARY}
        ${QT_QTSVG_LIBRARY}
)

INSTALL(TARGETS plexyjson DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)

ADD_SUBDIRECTORY(test)
