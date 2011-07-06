INCLUDE_DIRECTORIES(
   ${QT_INCLUDES}
   ${QT_QTOPENGL_INCLUDE_DIR}
   ${QT_QTNETWORK_INCLUDE_DIR}
   ${QT_QTTEST_INCLUDE_DIR}
   ${X11_INCLUDE_DIR}
   ${CMAKE_SOURCE_DIR}/lib
   )



########### Source files ###############
SET(sourceFiles
    main.cpp
    account.cpp
    accountmanager.cpp
    storagelayer.cpp
    adaptors/accountadaptor.cpp
    adaptors/accountmanageradaptor.cpp
    )

set(headerFiles
    account.h
    accountmanager.h
    storagelayer.h
    storagelayerinterface.h
    adaptors/accountadaptor.h
    adaptors/accountmanageradaptor.h
    )

set(QTMOC_SRCS
    account.h
    accountmanager.h
    storagelayer.h
    storagelayerinterface.h
    adaptors/accountadaptor.h
    adaptors/accountmanageradaptor.h
    )

QT4_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

IF(WIN32)
    SET(GUI_TYPE WIN32)
    ADD_DEFINITIONS(-DNODEFAULTLIB:library)
    SET (${QT_LIBRARIES} qtmain ${QT_LIBRARIES})
ENDIF(WIN32)

SET(libs
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    ${QT_AND_KDECORE_LIBS}
    )


########### Prepare files ###############
ADD_EXECUTABLE(socialplexyaccountman ${WIN_APPLICATION} ${sourceFiles} ${QT_MOC_SRCS})
#KDE4_ADD_PLUGIN(plexydesk ${raptor_plasmoid_PART_SRCS})

IF(MSVC)
    MSVC_SET_PROPER_CONSOLE_HANDLING(socialplexyaccountman)
ENDIF(MSVC)

TARGET_LINK_LIBRARIES(
    socialplexyaccountman
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    ${QT_QTSQL_LIBRARY}
    )

DBUS_ADD_ACTIVATION_SERVICE(org.plexydesk.SocialAccountsManager.service.in)


########### Install files ###############
INSTALL(TARGETS socialplexyaccountman  DESTINATION bin)
