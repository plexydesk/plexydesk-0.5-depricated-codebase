ADD_SUBDIRECTORY(img)

SET(sourceFiles
    interface.cpp
    imageplugin.cpp
    photowidget.cpp
    )

SET(headerFiles
    interface.h
    imageplugin.h
    photowidget.h
    )

SET(QTMOC_SRCS
    interface.h
    imageplugin.h
    photowidget.h
    )

QT5_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )

ADD_LIBRARY(photoframe SHARED ${sourceFiles} ${QT_MOC_SRCS})

QT5_USE_MODULES(photoframe Widgets Xml OpenGL Core WebKitWidgets Svg Declarative)

TARGET_LINK_LIBRARIES(photoframe
    ${PLEXY_UI_CORE_LIBRARY}
    )

INSTALL(TARGETS photoframe DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES photoframe.desktop DESTINATION share/plexy/ext/groups)
