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
SET(plexydesk_PART_SRCS
	main.cpp
)

SET (qtviz_MOC
	../plexycore/qt4/baserender.cpp
	../plexycore/qt4/desktopview.cpp
	../plexycore/qt4/pluginloader.cpp
	../plexycore/qt4/widgetfactory.cpp
	../plexycore/qt4/desktopwidget.cpp
	../plexycore/qt4/abstractplugininterface.cpp
	../plexycore/qt4/backdropfactory.cpp
	../extensions/backdrop/classicdrop/backdrop.cpp
	)
	
QT4_AUTOMOC(${qtviz_MOC})

########### prepare files ###############
ADD_EXECUTABLE(plexydesk ${plexydesk_PART_SRCS} ${qtviz_MOC})
#KDE4_ADD_PLUGIN(plexydesk ${raptor_plasmoid_PART_SRCS})


TARGET_LINK_LIBRARIES(
	plexydesk 
	qtviz
	${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
	${QT_AND_KDECORE_LIBS}
	)


########### install files ###############
INSTALL(TARGETS plexydesk DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
#INSTALL( FILES raptor.desktop  DESTINATION  ${DATA_INSTALL_DIR}/kicker/applets )
