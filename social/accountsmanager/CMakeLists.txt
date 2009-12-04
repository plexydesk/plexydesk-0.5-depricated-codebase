project(plexydesk)


set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
   ${QT_INCLUDES}
   ${QT_QTOPENGL_INCLUDE_DIR}
   ${QT_QTNETWORK_INCLUDE_DIR}
   ${QT_QTTEST_INCLUDE_DIR}
   ${X11_INCLUDE_DIR}
 )

include_directories(${CMAKE_SOURCE_DIR}/lib/)
########### source files ###############
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

qt4_wrap_cpp(QT_MOC_SRCS ${QTMOC_SRCS})


IF(WIN32)
SET(GUI_TYPE WIN32)
ADD_DEFINITIONS(-DNODEFAULTLIB:library)
SET (${QT_LIBRARIES} qtmain ${QT_LIBRARIES})
ENDIF(WIN32)

set(libs
	${QT_QTGUI_LIBRARY}
	${QT_QTCORE_LIBRARY}
   ${QT_QTOPENGL_LIBRARY}
	${QT_AND_KDECORE_LIBS}
)

########### prepare files ###############
ADD_EXECUTABLE(socialplexyaccountman ${sourceFiles} ${QT_MOC_SRCS})
#KDE4_ADD_PLUGIN(plexydesk ${raptor_plasmoid_PART_SRCS})

TARGET_LINK_LIBRARIES(
	socialplexyaccountman 
	qtviz
	${libs}
	${QT_QTSQL_LIBRARY}
)
include(PkgConfigGetVar)
dbus_add_activation_service(org.plexydesk.SocialAccountsManager.service.in)

########### install files ###############
INSTALL(TARGETS socialplexyaccountman  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
