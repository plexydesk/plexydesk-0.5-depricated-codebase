
set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	   ${QT_INCLUDES}
	   ${QT_QTOPENGL_INCLUDE_DIR}
	   ${QT_QTNETWORK_INCLUDE_DIR}
		 )


SET (wallpaper_CPP
    wallpaper.cpp
    wallpaperwidget.cpp
    )
    
SET (wallpaper_H
    wallpaper.h
    wallpaperwidget.h
    )
    
SOURCE_GROUP(source
    FILES
    ${wallpaper_CPP}
    )
    
SOURCE_GROUP(headers
    FILES
    ${wallpaper_H}
    )
    
SET (wallpaper_SRCS
    ${wallpaper_CPP}
    ${wallpaper_H}
    )

SET (plexicore_MOC
    ../../../plexycore/qt4/abstractplugininterface.cpp
    ../../../plexycore/qt4/pluginloader.cpp
    ../../../plexycore/qt4/desktopwidget.cpp
    )
    
QT4_AUTOMOC( ${plexicore_MOC})

add_library (wallpaperchange-win32 SHARED ${wallpaper_SRCS} ${plexicore_MOC})
QT4_AUTOMOC( ${wallpaper_SRCS}) 

TARGET_LINK_LIBRARIES(
	       wallpaperchange-win32
     		qtviz
	       ${QT_QTGUI_LIBRARY}
	       ${OPENGL_LIBRARIES}
	       ${QT_QTCORE_LIBRARY}
	       ${QT_QTOPENGL_LIBRARY}
       )

INSTALL(TARGETS wallpaperchange-win32 DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext)
INSTALL(FILES wallpaper-win32.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)
