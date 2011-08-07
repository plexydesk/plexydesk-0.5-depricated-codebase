ADD_SUBDIRECTORY(img)

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    )

SET(sourceFiles
    interface.cpp
    imageplugin.cpp
    )

SET(headerFiles
    interface.h
    imageplugin.h
    )

SET(QTMOC_SRCS
    interface.h
    imageplugin.h
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

ADD_LIBRARY(photoframe SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
    photoframe
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS photoframe DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES photoframe.desktop DESTINATION share/plexy/ext/groups)
