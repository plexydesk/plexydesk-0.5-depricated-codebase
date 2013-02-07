SET(sourceFiles
    interface.cpp
    mobilemonplugin.cpp
    photowidget.cpp
    notebookbase.cpp
    note.cpp
    )

SET(headerFiles
    interface.h
    mobilemonplugin.h
    photowidget.h
    notebookbase.h
    note.h
    )

SET(QTMOC_SRCS
    interface.h
    mobilemonplugin.h
    photowidget.h
    notebookbase.h
    note.h
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

ADD_LIBRARY(mobilemon SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(mobilemon
    ${PLEXY_UI_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS mobilemon DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES mobilemon.desktop DESTINATION share/plexy/ext/groups)
