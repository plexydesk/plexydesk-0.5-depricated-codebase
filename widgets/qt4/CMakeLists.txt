
SET (qtvizsrc 	
	baserender.cpp 
	desktopview.cpp
	netwm.cpp
		)

add_library (qtviz SHARED ${qtvizsrc})
QT4_AUTOMOC( ${qtvizsrc}) 
#SET_TARGET_PROPERTIES(plexy
#
#	·       ·       PROPERTIES
	#	#	·       ·       COMPILE_FLAGS ${CMAKE_SHARED_LIBRARY_CXX_FLAGS})
#

TARGET_LINK_LIBRARIES(
	       qtviz
	       GL
	       ${OPENGL_LIBRARIES}
	       ${QT_QTCORE_LIBRARY}
	       ${QT_AND_KDECORE_LIBS}
	       ${QT_QTOPENGL_LIBRARY}
	       ${KDE4_KDEUI_LIBS}
       )

INSTALL(TARGETS qtviz DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
