SET(sourceFiles
    irc.cpp
    )

SET(headerFiles
    irc.h
    )

SET(QTMOC_SRCS
    irc.h
    )

QT4_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )

ADD_LIBRARY(plexyirc SHARED ${sourceFiles} ${QT_MOC_SRCS})

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTXML_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    )

SET(libs
    ${QT_QTGUI_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTSVG_LIBRARY}
    )

TARGET_LINK_LIBRARIES(
    plexyirc
    ${libs}
    )

IF(NOT WIN32)
    INSTALL(TARGETS plexyirc DESTINATION lib)
ELSEIF(WIN32)
    INSTALL(TARGETS plexyirc DESTINATION bin)
ENDIF(NOT WIN32)
