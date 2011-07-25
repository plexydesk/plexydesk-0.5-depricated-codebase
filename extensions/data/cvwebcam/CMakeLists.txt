INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    ${OPENCV_INCLUDE_DIR}
    )

SET(sourceFiles
    webcam.cpp
    webcaminterface.cpp
    )

SET(headerFiles
    webcam.h
    webcaminterface.h
    )

SET(QTMOC_SRCS
    webcam.h
    webcaminterface.h
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
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    ${OPENCV_LIBRARIES}
    )

ADD_LIBRARY(webcamengine SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
    webcamengine
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS webcamengine DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES webcam.desktop DESTINATION share/plexy/ext/groups)
