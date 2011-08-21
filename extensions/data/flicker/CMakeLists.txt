SET(sourceFiles
    flicker.cpp
    flickerinterface.cpp
    )

SET(headerFiles
    flicker.h
    flickerinterface.h
    )

SET(QTMOC_SRCS
    flicker.h
    flickerinterface.h
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
    )

ADD_LIBRARY(flickerengine SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(flickerengine
        ${PLEXY_CORE_LIBRARY}
        ${libs}
)

INSTALL(TARGETS flickerengine DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES flicker.desktop DESTINATION share/plexy/ext/groups)
