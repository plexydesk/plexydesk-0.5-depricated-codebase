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

SET(libs
    ${QT_QTGUI_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTSVG_LIBRARY}
    )

TARGET_LINK_LIBRARIES(plexyirc
    ${libs}
    )

INSTALL(TARGETS plexyirc DESTINATION ${CMAKE_INSTALL_LIBDIR})
