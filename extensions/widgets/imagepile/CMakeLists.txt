
set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	   ${QT_INCLUDES}
	   ${QT_QTOPENGL_INCLUDE_DIR}
	   ${QT_QTNETWORK_INCLUDE_DIR}
	   ${QT_QTTEST_INCLUDE_DIR}
		 )


SET (imagepilsrc
	imageitem.cpp
	interface.cpp 
  pictureflow.cpp
  imageplugin.cpp
		)

SET (plexicore_MOC
  ../../../plexycore/qt4/desktopwidget.cpp
  ../../../plexycore/qt4/pluginloader.cpp
  ../../../plexycore/qt4/widgetplugin.cpp
  ../../../plexycore/qt4/abstractplugininterface.cpp
  ../../../plexycore/qt4/baseplugin.cpp
    )

QT4_AUTOMOC( ${plexicore_MOC})

add_library (imagepil SHARED ${imagepilsrc} ${plexicore_MOC})
QT4_AUTOMOC( ${imagepilsrc}) 

#SET_TARGET_PROPERTIES(plexy
#
#	·       ·       PROPERTIES
	#	#	·       ·       COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})
#

TARGET_LINK_LIBRARIES(
	       imagepil
				qtviz
			${QT_QTGUI_LIBRARY}
	       ${OPENGL_LIBRARIES}
	       ${QT_QTCORE_LIBRARY}
	       ${QT_QTOPENGL_LIBRARY}
       )

INSTALL(TARGETS imagepil DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES imagepil.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)

