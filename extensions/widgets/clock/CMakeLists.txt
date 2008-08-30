
set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	   ${QT_INCLUDES}
	   ${QT_QTOPENGL_INCLUDE_DIR}
	   ${QT_QTNETWORK_INCLUDE_DIR}
	   ${QT_QTTEST_INCLUDE_DIR}
		 )


SET (plexyclocksrc 	
	clock.cpp
	clockitem.cpp
	clockwidget.cpp
	clockinterface.cpp
		)

if (WIN32)
SET (plexicore_MOC
  ${CMAKE_SOURCE_DIR}/plexycore/qt4/pluginloader.cpp
  ${CMAKE_SOURCE_DIR}/plexycore/qt4/abstractplugininterface.cpp
  ${CMAKE_SOURCE_DIR}/plexycore/qt4/desktopwidget.cpp
  ${CMAKE_SOURCE_DIR}/plexycore/qt4/widgetplugin.cpp
  ${CMAKE_SOURCE_DIR}/plexycore/qt4/baseplugin.cpp
    )
    
QT4_AUTOMOC( ${plexycore_MOC}) 
endif(WIN32)
add_library (plexyclock SHARED ${plexyclocksrc} ${plexicore_MOC})
QT4_AUTOMOC( ${plexyclocksrc}) 

TARGET_LINK_LIBRARIES(
	       plexyclock
			qtviz
		   ${QT_QTGUI_LIBRARY}
	       ${OPENGL_LIBRARIES}
	       ${QT_QTCORE_LIBRARY}
	       ${QT_QTOPENGL_LIBRARY}
       )

INSTALL(TARGETS plexyclock DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES clock.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)

add_subdirectory(clock)
