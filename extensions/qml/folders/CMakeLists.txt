SET(sourceFiles
    plugin.cpp
    support.cpp
    )

SET(headerFiles
    support.h
    )

SET(QTMOC_SRCS
    support.h
    plugin.h
    )

QT4_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${QTMOC_SRCS}
    ${sourceFiles}
    ${headerFiles}
    )

SET(libs
    ${PLEXY_CORE_LIBRARY}
    ${QT_QTSCRIPT_LIBRARY}
    ${QT_QTDECLARATIVE_LIBRARY}
    )

ADD_LIBRARY(folderview  SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(folderview PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(folderview
    mimetype
    ${libs}
    )

# Note QT_IMPORTS_INSTALL_DIR is relative and handled in the main CMakeLists.txt file
INSTALL(TARGETS folderview DESTINATION ${QT_IMPORTS_INSTALL_DIR}/PlexyDesk/FolderView)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/qmldir  DESTINATION ${QT_IMPORTS_INSTALL_DIR}/PlexyDesk/FolderView)
