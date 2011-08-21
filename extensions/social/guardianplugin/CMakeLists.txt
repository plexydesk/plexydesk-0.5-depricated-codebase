SET(sourceFiles
    guardianplugin.cpp
    guardianplugininterface.cpp
    )

SET(headerFiles
    guardianplugin.h
    guardianinterface.h
    guardianplugininterface.h
    )

SET(QTMOC_SRCS
    guardianplugin.h
    guardianinterface.h
    guardianplugininterface.h
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
    ${QT_LIBRARIES}
    ${QT_QTNETWORK_LIBRARIES}
    ${QT_QTTEST_LIBRARIES}
    )

ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

ADD_LIBRARY(guardianplugin SHARED ${sourceFiles} ${QT_MOC_SRCS})


TARGET_LINK_LIBRARIES(guardianplugin
    qtviz
    ${libs}
    )

INSTALL(TARGETS guardianplugin DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES guardianplugin.desktop DESTINATION share/plexy/ext/groups)
