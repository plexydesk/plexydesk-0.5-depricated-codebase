
include(${QT_USE_FILE})
INCLUDE_DIRECTORIES(
	   ${QT_INCLUDES}
	   ${CMAKE_BINARY_DIR}/3rdparty/win32/ffmpeg/include
	   ${QT_QTOPENGL_INCLUDE_DIR}
	   ${QT_QTNETWORK_INCLUDE_DIR}
	   ${QT_QTTEST_INCLUDE_DIR}
		 )

set(sourceFiles
	video.cpp
	ivideointerface.cpp
	vplayer.cpp
)

set(headerFiles
	ivideointerface.h
	video.h
	vplayer.h
)

set(QTMOC_SRCS
	ivideointerface.h
	video.h
	vplayer.h
	../../../plexycore/qt4/desktopwidget.h
	../../../plexycore/qt4/pluginloader.h
	../../../plexycore/qt4/widgetplugin.h
	../../../plexycore/qt4/widgetinterface.h
	../../../plexycore/qt4/abstractplugininterface.h
	../../../plexycore/qt4/baseplugin.h
	../../../plexycore/qt4/backdropplugin.h
	../../../plexycore/qt4/dataplugin.h
	../../../plexycore/qt4/datainterface.h
	../../../plexycore/qt4/plexyconfig.h
)

qt4_wrap_cpp(QT_MOC_SRCS ${QTMOC_SRCS})

set(sourceFiles
	${sourceFiles}
	${headerFiles}
)

add_library (videoengine SHARED ${sourceFiles} ${QT_MOC_SRCS})

if(WIN32)
TARGET_LINK_LIBRARIES(
	       videoengine
	       qtviz
 	       ${CMAKE_SOURCE_DIR}/3rdparty/win32/ffmpeg/lib/avutil-49
	       ${CMAKE_SOURCE_DIR}/3rdparty/win32/ffmpeg/lib/avformat-51
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
