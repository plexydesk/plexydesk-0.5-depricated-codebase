ADD_SUBDIRECTORY(twit)

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    )

SET(sourceFiles
    videoentity.cpp
    twit.cpp
    twitinterface.cpp
    twitwidget.cpp
    )

SET(headerFiles
    videoentity.h
    twit.h
    twitinterface.h
    twitwidget.h
    )

SET(QTMOC_SRCS
    twit.h
    twitinterface.h
    twitwidget.h
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
    ${QT_QTOPENGL_LIBRARY}
    )

ADD_LIBRARY(plexytwit SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
    plexytwit
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS plexytwit DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES twit.desktop DESTINATION share/plexy/ext/groups)
