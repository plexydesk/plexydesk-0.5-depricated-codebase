PROJECT(plexydesk)

IF(COMMAND cmake_policy)
    CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    ${X11_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/base/qt4
    )


########### Source files ###############
SET(sourceFiles
    plexywindow.cpp
    main.cpp
    windowmanager.cpp
    )

SET(QTMOC_SRCS
    plexywindow.h
    windowmanager.h
    )

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
ADD_EXECUTABLE(plexydeskwm ${WIN_APPLICATION} ${sourceFiles} ${QT_MOC_SRCS})
#KDE4_ADD_PLUGIN(plexydeskwm ${raptor_plasmoid_PART_SRCS})

IF(MSVC)
    MSVC_SET_PROPER_CONSOLE_HANDLING(plexydeskwm)
ENDIF(MSVC)

TARGET_LINK_LIBRARIES(
    plexydeskwm
    ${PLEXY_CORE_LIBRARY}
    Xcomposite
    Xdamage
    ${libs}
    )


########### Install files ###############
INSTALL(TARGETS plexydeskwm DESTINATION bin)
#INSTALL(FILES raptor.desktop  DESTINATION  ${DATA_INSTALL_DIR}/kicker/applets)
