SET(widgetdemosrc
    widget.cpp
    widgetitem.cpp
    )

SET(plexycore_MOC
    ${CMAKE_SOURCE_DIR}/base/qt4/abstractplugininterface.cpp
    ${CMAKE_SOURCE_DIR}/base/qt4/pluginloader.cpp
    ${CMAKE_SOURCE_DIR}/base/qt4/desktopwidget.cpp
    )

QT4_AUTOMOC(${plexycore_MOC} ${widgetdemosrc})

ADD_LIBRARY(widgetdemo SHARED ${widgetdemosrc} ${plexycore_MOC})

TARGET_LINK_LIBRARIES(widgetdemo
    qtviz
    ${QT_QTGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    )

INSTALL(TARGETS widgetdemo DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext/widgets)
