ADD_SUBDIRECTORY(img)

SET(sourceFiles
    interface.cpp
    folderplugin.cpp
    )

SET(headerFiles
    interface.h
    folderplugin.h
    )

SET(QTMOC_SRCS
    interface.h
    folderplugin.h
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

ADD_LIBRARY(folderwidget SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(folderwidget
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS folderwidget DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES folderwidget.desktop DESTINATION share/plexy/ext/groups)
