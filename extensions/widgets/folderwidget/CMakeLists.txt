# photoframe widget already provides these
#ADD_SUBDIRECTORY(img)

SET(sourceFiles
    interface.cpp
    folderplugin.cpp
    iconwidgetview.cpp
    )

SET(headerFiles
    interface.h
    folderplugin.h
    iconwidgetview.h
    )

SET(QTMOC_SRCS
    interface.h
    folderplugin.h
    iconwidgetview.h
    )

QT5_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )

ADD_LIBRARY(folderwidget SHARED ${sourceFiles} ${QT_MOC_SRCS})

QT5_USE_MODULES(folderwidget Widgets Xml OpenGL Core WebKitWidgets Svg Declarative)

TARGET_LINK_LIBRARIES(folderwidget
    ${PLEXY_UI_CORE_LIBRARY}
    )

INSTALL(TARGETS folderwidget DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES folderwidget.desktop DESTINATION share/plexy/ext/groups)
