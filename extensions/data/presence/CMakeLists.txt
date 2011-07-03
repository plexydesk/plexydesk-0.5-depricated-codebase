FIND_PACKAGE(TelepathyQt4)

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    ${TELEPATHY_QT4_INCLUDE_DIR}
    )

SET(sourceFiles
    presence.cpp
    presenceinterface.cpp
    )

SET(headerFiles
    presence.h
    presenceinterface.h
    )

SET(QTMOC_SRCS
    presence.h
    presenceinterface.h
    )

QT4_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )

SET(libs
    ${QT_QTGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    ${TELEPATHY_QT4_LIBRARIES}
    )

ADD_LIBRARY(presenceengine SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
    presenceengine
    qtviz
    ${libs}
    )

INSTALL(TARGETS presenceengine DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext)
INSTALL(FILES presence.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups)
