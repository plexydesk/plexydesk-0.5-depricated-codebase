INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    )


SET(widgetdemosrc
    widget.cpp
    widgetitem.cpp
    )

SET(plexicore_MOC
    ${CMAKE_SOURCE_DIR}/plexycore/qt4/abstractplugininterface.cpp
    ${CMAKE_SOURCE_DIR}/plexycore/qt4/pluginloader.cpp
    ${CMAKE_SOURCE_DIR}/plexycore/qt4/desktopwidget.cpp
    )

QT4_AUTOMOC( ${plexicore_MOC})

ADD_LIBRARY(widgetdemo SHARED ${widgetdemosrc} ${plexicore_MOC})
QT4_AUTOMOC( ${widgetdemosrc})

#SET_TARGET_PROPERTIES(plexy PROPERTIES
#                      COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})

TARGET_LINK_LIBRARIES(
    widgetdemo
    qtviz
    ${QT_QTGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    )

INSTALL(TARGETS widgetdemo DESTINATION lib/plexyext/widgets)
