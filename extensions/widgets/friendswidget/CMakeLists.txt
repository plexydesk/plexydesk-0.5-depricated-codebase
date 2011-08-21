ADD_SUBDIRECTORY(friends)

SET(plexyfriendssrc
    friends.cpp
    friendswidget.cpp
    frienditem.cpp
    )

SET(plexicore_MOC
    ${CMAKE_SOURCE_DIR}/base/qt4/pluginloader.cpp
    ${CMAKE_SOURCE_DIR}/base/qt4/abstractplugininterface.cpp
    ${CMAKE_SOURCE_DIR}/base/qt4/desktopwidget.cpp
    )

QT4_AUTOMOC( ${plexicore_MOC})

ADD_LIBRARY(plexyfriends SHARED ${plexyfriendssrc} ${plexicore_MOC})
QT4_AUTOMOC( ${plexyfriendssrc})

TARGET_LINK_LIBRARIES(plexyfriends
    qtviz
    ${QT_QTGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    )

INSTALL(TARGETS plexyfriends DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES friends.desktop DESTINATION share/plexy/ext/groups)
