SET (rssenginesrc
    rss.cpp
    )

SET (plexicore_MOC
    ${CMAKE_SOURCE_DIR}/base/qt4/abstractplugininterface.cpp
    ${CMAKE_SOURCE_DIR}/base/qt4/pluginloader.cpp
    )

QT4_AUTOMOC(${plexicore_MOC} ${rssenginesrc})

ADD_LIBRARY(rssengine SHARED ${rssenginesrc} ${plexicore_MOC})

TARGET_LINK_LIBRARIES(rssengine
        qtviz
        ${QT_QTGUI_LIBRARY}
        ${OPENGL_LIBRARIES}
        ${QT_QTCORE_LIBRARY}
        ${QT_QTNETWORK_LIBRARY}
        ${QT_QTOPENGL_LIBRARY}
        )

INSTALL(TARGETS rssengine DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES rss.desktop DESTINATION share/plexy/ext/groups)
