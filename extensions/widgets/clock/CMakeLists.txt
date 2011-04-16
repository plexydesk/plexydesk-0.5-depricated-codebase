INCLUDE_DIRECTORIES(
	${QT_INCLUDES}
	${QT_QTOPENGL_INCLUDE_DIR}
	${QT_QTNETWORK_INCLUDE_DIR}
	${QT_QTTEST_INCLUDE_DIR}
)

SET(sourceFiles
	clock.cpp
	clockwidget.cpp
)

set(headerFiles
	clock.h
	clockwidget.h
	clockinterface.h
)

set(QTMOC_SRCS
	clock.h
	clockwidget.h
	clockinterface.h
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
	plexydeskcore
	${libs}
)

INSTALL(TARGETS plexyclock DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES clock.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)

add_subdirectory(clock)
