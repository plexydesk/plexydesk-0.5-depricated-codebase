PROJECT(plexyfrens)

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    ${X11_INCLUDE_DIR}
    )


########### Source files ###############
SET(sourceFiles
    desktopview.cpp
    main.cpp
    )

SET(QTMOC_SRCS desktopview.h)

QT4_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

IF(WIN32)
    SET(GUI_TYPE WIN32)
    ADD_DEFINITIONS(-DNODEFAULTLIB:library)
    SET(${QT_LIBRARIES} qtmain ${QT_LIBRARIES})
ENDIF(WIN32)

SET(libs
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    ${QT_AND_KDECORE_LIBS}
    )

########### Prepare files ###############
ADD_EXECUTABLE(plexyfrens ${WIN_APPLICATION} ${sourceFiles} ${QT_MOC_SRCS})

IF(MSVC)
    MSVC_SET_PROPER_CONSOLE_HANDLING(plexyfrens)
EMDIF(MSVC)

TARGET_LINK_LIBRARIES(
    plexyfrens
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )


########### Install files ###############
INSTALL(TARGETS plexyfrens DESTINATION bin)
