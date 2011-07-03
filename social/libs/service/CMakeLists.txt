INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    )

SET(sourceFiles
    socialaccessmanager.cpp
    )

SET(headerFiles
    socialaccessmanager.h
    )

SET(QTMOC_SRCS
    socialaccessmanager.h
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
    ${DBUS_LIB}
    ${QT_QTNETWORK_LIBRARY}
    )

ADD_LIBRARY(plexysocialservice SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
    plexysocialservice
    ${libs}
    )

IF (NOT WIN32)
    INSTALL(TARGETS plexysocialservice DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
ELSEIF(WIN32)
    INSTALL(TARGETS plexysocialservice DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
ENDIF(NOT WIN32)
