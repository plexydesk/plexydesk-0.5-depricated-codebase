INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    )

SET(sourceFiles
    weatherplugin.cpp
    weatherplugininterface.cpp
    )

SET(headerFiles
    weatherplugin.h
    weatherinterface.h
    weatherplugininterface.h
    )

SET(QTMOC_SRCS
    weatherplugin.h
    weatherinterface.h
    weatherplugininterface.h
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

ADD_LIBRARY(weatherplugin SHARED ${sourceFiles} ${QT_MOC_SRCS})
#ADD_LIBRARY($$qtLibraryTarget(weatherplugin) ${$$qtLibraryTarget(weatherplugin)_sources})


TARGET_LINK_LIBRARIES(
    weatherplugin
    qtviz
    ${libs}
    )

INSTALL(TARGETS weatherplugin DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES weatherplugin.desktop DESTINATION share/plexy/ext/groups)
