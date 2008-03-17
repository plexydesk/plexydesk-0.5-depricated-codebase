
include(${QT_USE_FILE})
INCLUDE_DIRECTORIES(
	   ${QT_INCLUDES}
	   ${QT_QTOPENGL_INCLUDE_DIR}
	   ${QT_QTNETWORK_INCLUDE_DIR}
	   ${QT_QTTEST_INCLUDE_DIR}
		 )


SET (videoenginesrc video.cpp  videointerface.cpp  vplayer.cpp )
		

add_library (videoengine SHARED ${videoenginesrc})
QT4_AUTOMOC( ${videoenginesrc}) 

#SET_TARGET_PROPERTIES(plexy
#
#	·       ·       PROPERTIES
	#	#	·       ·       COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})
#

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

INSTALL(TARGETS videoengine DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES videoengine.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)
