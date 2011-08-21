ADD_SUBDIRECTORY(googleweather)

SET(sourceFiles
    googleweather.cpp
    googleweatherinterface.cpp
    googleweatherwidget.cpp
    socialqdbusplugindata.cpp
    )

SET(headerFiles
    googleweather.h
    googleweatherinterface.h
    googleweatherwidget.h
    socialqdbusplugindata.h
    )

SET(QTMOC_SRCS
    googleweather.h
    googleweatherinterface.h
    googleweatherwidget.h
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

ADD_LIBRARY(googleweather SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(googleweather
    qtviz
    ${libs}
    )

INSTALL(TARGETS googleweather DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES googleweather.desktop DESTINATION share/plexy/ext/groups)
