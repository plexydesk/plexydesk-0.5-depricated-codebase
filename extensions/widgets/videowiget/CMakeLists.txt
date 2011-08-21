SET(videowidgetsrc
    video.cpp
    videoitem.cpp
    )

ADD_LIBRARY(videowidget SHARED ${videowidgetsrc})

QT4_AUTOMOC(${videowidgetsrc})

TARGET_LINK_LIBRARIES(videowidget
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    )

INSTALL(TARGETS videowidget DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext/widgets)
