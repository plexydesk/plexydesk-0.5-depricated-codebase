SET(sourceFiles
    qdeclarativefolderlistmodel.cpp
    )

SET(headerFiles
    plugin.h
    qdeclarativefolderlistmodel.h
    )

QT4_WRAP_CPP(QT_MOC_SRCS ${headerFiles})

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

ADD_LIBRARY(folderlistmodel  SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(folderlistmodel PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(folderlistmodel
    mimetype
    ${libs}
    )

# Note QT_IMPORTS_INSTALL_DIR is relative and handled in the main CMakeLists.txt file
INSTALL(TARGETS folderlistmodel DESTINATION ${QT_IMPORTS_INSTALL_DIR}/PlexyDesk/FolderListModel)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/qmldir  DESTINATION ${QT_IMPORTS_INSTALL_DIR}/PlexyDesk/FolderListModel)
