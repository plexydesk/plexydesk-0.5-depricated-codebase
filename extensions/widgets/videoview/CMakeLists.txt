PROJECT(videoview)

ADD_SUBDIRECTORY(img)

SET(sourceFiles
    videoitem.cpp
    videointerface.cpp
    videoplugin.cpp
    )

SET(headerFiles
    videoitem.h
    videointerface.h
    videoplugin.h
    )

SET(QTMOC_SRCS
    videoitem.h
    videointerface.h
    videoplugin.h
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

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    )

ADD_LIBRARY(videoview SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
    videoview
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS videoview DESTINATION lib/plexyext)
INSTALL(FILES videoview.desktop DESTINATION share/plexy/ext/groups)
