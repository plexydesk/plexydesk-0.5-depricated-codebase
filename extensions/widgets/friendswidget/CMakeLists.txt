ADD_SUBDIRECTORY(friends)

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    )


SET(plexyfriendssrc
    friends.cpp
    friendswidget.cpp
    frienditem.cpp
    )

SET(plexicore_MOC
    ${CMAKE_SOURCE_DIR}/plexycore/qt4/pluginloader.cpp
    ${CMAKE_SOURCE_DIR}/plexycore/qt4/abstractplugininterface.cpp
    ${CMAKE_SOURCE_DIR}/plexycore/qt4/desktopwidget.cpp
    )

QT4_AUTOMOC( ${plexicore_MOC})

ADD_LIBRARY(plexyfriends SHARED ${plexyfriendssrc} ${plexicore_MOC})
QT4_AUTOMOC( ${plexyfriendssrc})

#SET_TARGET_PROPERTIES(plexy PROPERTIES
#                      COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})


TARGET_LINK_LIBRARIES(
    plexyfriends
    qtviz
    ${QT_QTGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    )

INSTALL(TARGETS plexyfriends DESTINATION lib/plexyext)
INSTALL(FILES friends.desktop DESTINATION share/plexy/ext/groups)
