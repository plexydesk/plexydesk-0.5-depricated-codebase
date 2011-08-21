SET (localphotosenginesrc
    localphotos.cpp
    localphotosinterface.cpp
    )

SET (plexicore_MOC
    ${CMAKE_SOURCE_DIR}/base/qt4/pluginloader.cpp
    ${CMAKE_SOURCE_DIR}/base/qt4/abstractplugininterface.cpp
    ${CMAKE_SOURCE_DIR}/base/qt4/dataplugin.cpp
    ${CMAKE_SOURCE_DIR}/base/qt4/baseplugin.cpp
    )

QT4_AUTOMOC(${plexicore_MOC} ${localphotosenginesrc})

ADD_LIBRARY(localphotosengine SHARED ${localphotosenginesrc} ${plexicore_MOC})

TARGET_LINK_LIBRARIES(localphotosengine
    qtviz
    ${QT_QTGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    )

INSTALL(TARGETS localphotosengine DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES localphotos.desktop DESTINATION share/plexy/ext/groups)
