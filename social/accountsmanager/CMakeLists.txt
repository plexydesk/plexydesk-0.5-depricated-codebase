INCLUDE_DIRECTORIES(
   ${X11_INCLUDE_DIR}
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
ENDIF(WIN32)

SET(libs
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    ${QT_QTMAIN_LIBRARY}
    ${QT_QTSQL_LIBRARY}
    )


########### Prepare files ###############
ADD_EXECUTABLE(socialplexyaccountman ${WIN_APPLICATION} ${sourceFiles} ${QT_MOC_SRCS})

IF(MSVC)
    MSVC_SET_PROPER_CONSOLE_HANDLING(socialplexyaccountman)
ENDIF(MSVC)

TARGET_LINK_LIBRARIES(socialplexyaccountman
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )

DBUS_ADD_ACTIVATION_SERVICE(org.plexydesk.SocialAccountsManager.service.in)


########### Install files ###############
INSTALL(TARGETS socialplexyaccountman  DESTINATION bin)
