INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    )

SET(sourceFiles
    qwebviewitem.cpp
    )

SET(headerFiles
    qwebviewitem.h
    config.h
    )

SET(MOC_SRCS
    qwebviewitem.h
    )

QT4_WRAP_CPP(QT_MOC_SRCS ${MOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )

ADD_LIBRARY (webqgv SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(webqgv PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

SET(libs
    ${QT_QTWEBKIT_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTSVG_LIBRARY}
    )

TARGET_LINK_LIBRARIES(
    webqgv
    ${libs}
    )

INSTALL(TARGETS webqgv DESTINATION lib)
