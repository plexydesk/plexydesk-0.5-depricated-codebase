ADD_SUBDIRECTORY(img)

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    )

SET(sourceFiles
    imageitem.cpp
    interface.cpp
    pictureflow.cpp
    imageplugin.cpp
    )

SET(headerFiles
    imageitem.h
    interface.h
    pictureflow.h
    imageplugin.h
    )

SET(QTMOC_SRCS
    imageitem.h
    interface.h
    pictureflow.h
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

ADD_LIBRARY(imagepil SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
    imagepil
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS imagepil DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES imagepil.desktop DESTINATION share/plexy/ext/groups)
