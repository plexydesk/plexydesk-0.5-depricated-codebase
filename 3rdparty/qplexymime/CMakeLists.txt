# Check if we use any Debug in the final release and if so compile the tests
IF(CMAKE_BUILD_TYPE MATCHES ".*Deb.*")
    ADD_SUBDIRECTORY(test)
ENDIF(CMAKE_BUILD_TYPE MATCHES ".*Deb.*")


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
    ${PLEXY_UI_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS plexymime DESTINATION ${CMAKE_INSTALL_LIBDIR})


IF (APPLE OR WIN32)
    FILE(GLOB XML_FILES *.xml)
    INSTALL(FILES ${XML_FILES} mimetypes DESTINATION share/plexy/mime)
ENDIF (APPLE OR WIN32)
