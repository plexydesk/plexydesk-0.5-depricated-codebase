ADD_SUBDIRECTORY(guardiannews)

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    )

SET(sourceFiles
    guardiannews.cpp
    guardiannewsinterface.cpp
    guardiannewswidget.cpp
    socialqdbusplugindata.cpp
    )

SET(headerFiles
    guardiannews.h
    guardiannewsinterface.h
    guardiannewswidget.h
    socialqdbusplugindata.h
    )

SET(QTMOC_SRCS
    guardiannews.h
    guardiannewsinterface.h
    guardiannewswidget.h
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

ADD_LIBRARY(guardiannews SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
    guardiannews
    qtviz
    ${libs}
    )

INSTALL(TARGETS guardiannews DESTINATION lib/plexyext)
INSTALL(FILES guardiannews.desktop DESTINATION share/plexy/ext/groups)
