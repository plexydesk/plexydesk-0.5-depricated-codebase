ADD_SUBDIRECTORY(tests)

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTXMLPATTERNS_INCLUDE_DIR}
    )

SET(resourceFiles
    qplexymime.qrc
    )

QT4_ADD_RESOURCES(QT_RCC_SRCS ${resourceFiles})

SET(sourceFiles
    qplexymime.cpp
    )

SET(headerFiles
    qplexymime.h
    )

SET(QTMOC_SRCS
    qplexymime.h
    )

QT4_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )

SET(libs
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTXMLPATTERNS_LIBRARY}
    )

ADD_LIBRARY(plexymime SHARED
    ${sourceFiles}
    ${QT_MOC_SRCS}
    ${QT_RCC_SRCS}
    )

IF(MINGW)
    SET_TARGET_PROPERTIES(plexymime PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(plexymime
    ${libs}
    )

IF (NOT WIN32)
    INSTALL(TARGETS plexymime DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
ELSEIF(WIN32)
    INSTALL(TARGETS plexymime DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
ENDIF(NOT WIN32)

IF (APPLE OR WIN32)
    FILE(GLOB XML_FILES *.xml)
    INSTALL(FILES ${XML_FILES}  DESTINATION ${CMAKE_INSTALL_PREFIX}/plexy/mime)
ENDIF (APPLE OR WIN32)
