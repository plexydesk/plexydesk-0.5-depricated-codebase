
set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	${QT_INCLUDES}
	${QT_QTOPENGL_INCLUDE_DIR}
	${QT_QTNETWORK_INCLUDE_DIR}
	${QT_QTTEST_INCLUDE_DIR}
)

SET (sourceFiles
	videoentity.cpp
	youtube.cpp
	youtubeinterface.cpp
	youtubewidget.cpp
)

set(headerFiles
	videoentity.h
	youtube.h
	youtubeinterface.h
	youtubewidget.h
)

set(QTMOC_SRCS
	youtube.h
	youtubeinterface.h
	youtubewidget.h
)

qt4_wrap_cpp(QT_MOC_SRCS ${QTMOC_SRCS})

set(sourceFiles
	${sourceFiles}
	${headerFiles}
)

set(libs
	${QT_QTGUI_LIBRARY}
	${OPENGL_LIBRARIES}
	${QT_QTCORE_LIBRARY}
	${QT_QTOPENGL_LIBRARY}
)

add_library (plexyyoutube SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
	plexyyoutube
	qtviz
	${libs}
)

INSTALL(TARGETS plexyyoutube DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES youtube.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)

add_subdirectory(youtube)
