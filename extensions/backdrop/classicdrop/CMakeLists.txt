
set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	${QT_INCLUDES}
	${QT_QTOPENGL_INCLUDE_DIR}
	${QT_QTNETWORK_INCLUDE_DIR}
	${QT_QTTEST_INCLUDE_DIR}
 )


set(sourceFiles
	backdrop.cpp
	classicinterface.cpp
)

set(headerFiles
	backdrop.h
	classicinterface.h
)
#XXX: Do we reall need this ??
set(QTMOC_SRCS
	backdrop.h
	classicinterface.h
	../../../base/qt4/desktopwidget.h
	../../../base/qt4/pluginloader.h
	../../../base/qt4/widgetplugin.h
	../../../base/qt4/widgetinterface.h
	../../../base/qt4/abstractplugininterface.h
	../../../base/qt4/baseplugin.h
	../../../base/qt4/backdropplugin.h
	../../../base/qt4/dataplugin.h
	../../../base/qt4/datainterface.h
	../../../base/qt4/plexyconfig.h
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

add_library (classicbackdrop SHARED ${sourceFiles})

TARGET_LINK_LIBRARIES(
	classicbackdrop
	qtviz
	${libs}
)

INSTALL(TARGETS classicbackdrop DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES classic.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)
