SET(exampleenginesrc
    example.cpp
    exampleinterface.cpp
    )

SET(plexicore_MOC
    ${CMAKE_SOURCE_DIR}/base/qt4/pluginloader.cpp
    ${CMAKE_SOURCE_DIR}/base/qt4/abstractplugininterface.cpp
    ${CMAKE_SOURCE_DIR}/base/qt4/dataplugin.cpp
    ${CMAKE_SOURCE_DIR}/base/qt4/baseplugin.cpp
    )

QT4_AUTOMOC(${plexicore_MOC})

ADD_LIBRARY(exampleengine SHARED ${exampleenginesrc} ${plexicore_MOC})
QT4_AUTOMOC(${exampleenginesrc})

TARGET_LINK_LIBRARIES(exampleengine
    qtviz
    ${QT_QTGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    )

INSTALL(TARGETS exampleengine DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES example.desktop DESTINATION share/plexy/ext/groups)
