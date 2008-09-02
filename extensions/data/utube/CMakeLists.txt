
set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	   ${QT_INCLUDES}
	   ${QT_QTOPENGL_INCLUDE_DIR}
	   ${QT_QTNETWORK_INCLUDE_DIR}
	   ${QT_QTTEST_INCLUDE_DIR}
		 )


SET (utubeenginesrc 	
    utube.cpp
    youtubedatainterface.cpp
		)

SET (plexicore_MOC
    ${CMAKE_SOURCE_DIR}/plexycore/qt4/abstractplugininterface.cpp
    ${CMAKE_SOURCE_DIR}/plexycore/qt4/pluginloader.cpp
	${CMAKE_SOURCE_DIR}/plexycore/qt4/dataplugin.cpp
	${CMAKE_SOURCE_DIR}/plexycore/qt4/baseplugin.cpp
    )
    
QT4_AUTOMOC( ${plexicore_MOC})

add_library (utubeengine SHARED ${utubeenginesrc} ${plexicore_MOC})
QT4_AUTOMOC( ${utubeenginesrc}) 


TARGET_LINK_LIBRARIES(
	       utubeengine
	       qtviz
	       ${QT_QTGUI_LIBRARY}
	       ${OPENGL_LIBRARIES}
	       ${QT_QTCORE_LIBRARY}
	       ${QT_QTNETWORK_LIBRARY}
	       ${QT_QTOPENGL_LIBRARY}
       )

INSTALL(TARGETS utubeengine DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES utube.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)
