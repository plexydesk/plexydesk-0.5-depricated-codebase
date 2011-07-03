INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    )

SET(videowidgetsrc
    video.cpp
    videoitem.cpp
    )

ADD_LIBRARY(videowidget SHARED ${videowidgetsrc})

QT4_AUTOMOC(${videowidgetsrc})

#SET_TARGET_PROPERTIES(plexy PROPERTIES
#                      COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})

TARGET_LINK_LIBRARIES(
    videowidget
    GL
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    )

INSTALL(TARGETS videowidget DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/widgets)
