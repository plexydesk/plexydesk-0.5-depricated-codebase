
include(${QT_USE_FILE})
INCLUDE_DIRECTORIES(
	   ${QT_INCLUDES}
	   ${QT_QTOPENGL_INCLUDE_DIR}
	   ${QT_QTNETWORK_INCLUDE_DIR}
	   ${QT_QTTEST_INCLUDE_DIR}
		 )

SET (videoenginesrc video.cpp  videointerface.cpp  vplayer.cpp )

SET (plexicore_MOC
  ${CMAKE_SOURCE_DIR}/plexycore/qt4/dataplugin.cpp
  ${CMAKE_SOURCE_DIR}/plexycore/qt4/abstractplugininterface.cpp
  ${CMAKE_SOURCE_DIR}/plexycore/qt4/baseplugin.cpp
    )
    
QT4_AUTOMOC( ${plexicore_MOC})
		
add_library (videoengine SHARED ${videoenginesrc} ${plexicore_MOC})
QT4_AUTOMOC( ${videoenginesrc}) 

#SET_TARGET_PROPERTIES(plexy
#
#	·       ·       PROPERTIES
	#	#	·       ·       COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})
#

if(WIN32)
TARGET_LINK_LIBRARIES(
	       videoengine
	       qtviz
 	       ${CMAKE_SOURCE_DIR}/3rdparty/win32/ffmpeg/lib/avutil-49
	       ${CMAKE_SOURCE_DIR}/3rdparty/win32/ffmpeg/lib/avformat-50
	       ${CMAKE_SOURCE_DIR}/3rdparty/win32/ffmpeg/lib/avcodec-51
	       ${QT_QTGUI_LIBRARY}
	       ${OPENGL_LIBRARIES}
	       ${QT_QTCORE_LIBRARY}
	       ${QT_QTNETWORK_LIBRARY}
	       ${QT_QTOPENGL_LIBRARY}
       )
else(WIN32)
TARGET_LINK_LIBRARIES(
	       videoengine
	       qtviz
		avutil 
		avformat
		avcodec 
	       ${QT_QTGUI_LIBRARY}
	       ${OPENGL_LIBRARIES}
	       ${QT_QTCORE_LIBRARY}
	       ${QT_QTNETWORK_LIBRARY}
	       ${QT_QTOPENGL_LIBRARY}
       )
endif(WIN32)

INSTALL(TARGETS videoengine DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES videoengine.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)
