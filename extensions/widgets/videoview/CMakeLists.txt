
set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	   ${QT_INCLUDES}
	   ${QT_QTOPENGL_INCLUDE_DIR}
	   ${QT_QTNETWORK_INCLUDE_DIR}
	   ${QT_QTTEST_INCLUDE_DIR}
		 )


SET (videoviewsrc
	videoitem.cpp
	interface.cpp 
    videoplugin.cpp
		)

SET (plexicore_MOC
  ${CMAKE_SOURCE_DIR}/plexycore/qt4/desktopwidget.cpp
  ${CMAKE_SOURCE_DIR}/plexycore/qt4/pluginloader.cpp
  ${CMAKE_SOURCE_DIR}/plexycore/qt4/widgetplugin.cpp
  ${CMAKE_SOURCE_DIR}/plexycore/qt4/abstractplugininterface.cpp
  ${CMAKE_SOURCE_DIR}/plexycore/qt4/baseplugin.cpp
    )

QT4_AUTOMOC( ${plexicore_MOC})

add_library (videoview SHARED ${videoviewsrc} ${plexicore_MOC})
QT4_AUTOMOC( ${videoviewsrc}) 

#SET_TARGET_PROPERTIES(plexy
#
#	·       ·       PROPERTIES
	#	#	·       ·       COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})
#

TARGET_LINK_LIBRARIES(
	       videoview
				qtviz
			${QT_QTGUI_LIBRARY}
	       ${OPENGL_LIBRARIES}
	       ${QT_QTCORE_LIBRARY}
	       ${QT_QTOPENGL_LIBRARY}
       )

INSTALL(TARGETS videoview DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES videoview.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)

add_subdirectory(img)

