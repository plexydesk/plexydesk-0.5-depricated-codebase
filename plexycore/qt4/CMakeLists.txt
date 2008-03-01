
set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	   ${QT_INCLUDES}
	   ${QT_QTOPENGL_INCLUDE_DIR}
	   ${QT_QTNETWORK_INCLUDE_DIR}
	   ${QT_QTTEST_INCLUDE_DIR}
		 )


SET (qtvizsrc 	
	#	baserender.cpp 
	desktopview.cpp
	pluginloader.cpp
	netwm.cpp
	backdropinterface.cpp
	fakemime.cpp
	#	extensionfactory.cpp
	#	backdropfactory.cpp
#	widgetfactory.cpp
	desktopwidget.cpp
	plexyconfig.cpp
		)

add_library (qtviz SHARED ${qtvizsrc})
QT4_AUTOMOC( ${qtvizsrc}) 


TARGET_LINK_LIBRARIES(
	       qtviz
	       ${QT_QTGUI_LIBRARY}
	       ${OPENGL_LIBRARIES}
	       ${QT_QTCORE_LIBRARY}
	       ${QT_QTOPENGL_LIBRARY}
       )

INSTALL(TARGETS qtviz DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
