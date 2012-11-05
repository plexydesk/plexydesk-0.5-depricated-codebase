SET(sourceFiles
    authplugin.cpp
    authwidgetinterface.cpp
    authwidget.cpp
    facebookcontactui.cpp
    contactlist.cpp
    contactlistitem.cpp
    )

SET(headerFiles
    authplugin.h
    authwidgetinterface.h
    authwidget.h
    facebookcontactui.h
    contactlist.h
    contactlistitem.h
)

SET(QTMOC_SRCS
    authplugin.h
    authwidgetinterface.h
    authwidget.h
    facebookcontactui.h
    contactlist.h
    contactlistitem.h
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

ADD_LIBRARY(plexyauth SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(plexyauth
    ${PLEXY_UI_CORE_LIBRARY}
    plexyjson
    webqgv
    ${libs}
    )

INSTALL(TARGETS plexyauth DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES authwidget.desktop DESTINATION share/plexy/ext/groups)
