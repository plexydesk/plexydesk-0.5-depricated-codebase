
set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	   ${QT_INCLUDES}
	   ${QT_QTOPENGL_INCLUDE_DIR}
	   ${QT_QTNETWORK_INCLUDE_DIR}
	   ${QT_QTTEST_INCLUDE_DIR}
		 )


SET (classicbackdropsrc 	
		backdrop.cpp
    classicinterface.cpp
		)
		
SET (plexicore_MOC
    ../../../plexycore/qt4/backdropplugin.cpp
    ../../../plexycore/qt4/baseplugin.cpp
    ../../../plexycore/qt4/abstractplugininterface.cpp
    ../../../plexycore/qt4/pluginloader.cpp
    )
    
QT4_AUTOMOC( ${plexicore_MOC})

QT4_AUTOMOC( ${classicbackdropsrc}) 
add_library (classicbackdrop SHARED ${classicbackdropsrc} ${plexicore_MOC})


TARGET_LINK_LIBRARIES(
	       classicbackdrop
	       qtviz
	       ${QT_QTGUI_LIBRARY}
	       ${OPENGL_LIBRARIES}
	       ${QT_QTCORE_LIBRARY}
	       ${QT_QTOPENGL_LIBRARY}
       )


INSTALL(TARGETS classicbackdrop DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES classic.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)
