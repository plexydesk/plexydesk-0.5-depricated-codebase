
set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	${QT_INCLUDES}
	${QT_QTOPENGL_INCLUDE_DIR}
	${QT_QTNETWORK_INCLUDE_DIR}
	${QT_QTTEST_INCLUDE_DIR}
)

SET(sourceFiles
	clock.cpp
	clockitem.cpp
	clockwidget.cpp
)

set(headerFiles
	clock.h
	clockitem.h
	clockwidget.h
	clockinterface.h
)

set(QTMOC_SRCS
	clock.h
	clockitem.h
	clockwidget.h
	clockinterface.h
	../../../plexycore/qt4/desktopwidget.h
	../../../plexycore/qt4/pluginloader.h
	../../../plexycore/qt4/widgetplugin.h
	../../../plexycore/qt4/widgetinterface.h
	../../../plexycore/qt4/abstractplugininterface.h
	../../../plexycore/qt4/baseplugin.h
	../../../plexycore/qt4/dataplugin.h
	../../../plexycore/qt4/datainterface.h
	../../../plexycore/qt4/plexyconfig.h
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

add_library (plexyclock SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
	plexyclock
	qtviz
	${libs}
)

INSTALL(TARGETS plexyclock DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES clock.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)

add_subdirectory(clock)
