INCLUDE_DIRECTORIES(
    ${X11_INCLUDE_DIR}
    )


########### Source files ###############
SET(sourceFiles
    main.cpp
    daemon.cpp
    )

QT4_WRAP_CPP(QT_MOC_SRCS daemon.h)

IF(WIN32)
    SET(GUI_TYPE WIN32)
    ADD_DEFINITIONS(-DNODEFAULTLIB:library)
ENDIF(WIN32)

SET(libs
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    ${QT_QTMAIN_LIBRARY}
    )

########### prepare files ###############
ADD_EXECUTABLE(socialplexydaemon ${WIN_APPLICATION} ${sourceFiles} ${QT_MOC_SRCS})

IF(MSVC)
    MSVC_SET_PROPER_CONSOLE_HANDLING(socialplexydaemon)
ENDIF(MSVC)

TARGET_LINK_LIBRARIES(socialplexydaemon
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )

DBUS_ADD_ACTIVATION_SERVICE(org.plexydesk.social.service.in)

########### Install files ###############
INSTALL(TARGETS socialplexydaemon  DESTINATION bin)
