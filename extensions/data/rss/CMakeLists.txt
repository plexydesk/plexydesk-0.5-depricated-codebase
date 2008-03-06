
set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	   ${QT_INCLUDES}
	   ${QT_QTOPENGL_INCLUDE_DIR}
	   ${QT_QTNETWORK_INCLUDE_DIR}
	   ${QT_QTTEST_INCLUDE_DIR}
		 )


SET (rssenginesrc 	
	rss.cpp
		)

SET (plexicore_MOC
    ../../../plexycore/qt4/abstractplugininterface.cpp
    ../../../plexycore/qt4/pluginloader.cpp
    )
    
QT4_AUTOMOC( ${plexicore_MOC})

add_library (rssengine SHARED ${rssenginesrc} ${plexicore_MOC})
QT4_AUTOMOC( ${rssenginesrc}) 

#SET_TARGET_PROPERTIES(plexy
#
#	·       ·       PROPERTIES
	#	#	·       ·       COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})
#

TARGET_LINK_LIBRARIES(
	       rssengine
	       qtviz
	       ${QT_QTGUI_LIBRARY}
	       ${OPENGL_LIBRARIES}
	       ${QT_QTCORE_LIBRARY}
	       ${QT_QTNETWORK_LIBRARY}
	       ${QT_QTOPENGL_LIBRARY}
       )

INSTALL(TARGETS rssengine DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES rss.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)
