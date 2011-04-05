
set(QT_USE_OPENGL TRUE)

include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	${QT_INCLUDES}
	${QT_QTOPENGL_INCLUDE_DIR}
	${QT_QTNETWORK_INCLUDE_DIR}
	${QT_QTTEST_INCLUDE_DIR}
)

SET (sourceFiles
	googleweather.cpp
	googleweatherinterface.cpp
	googleweatherwidget.cpp
	socialqdbusplugindata.cpp
)

set(headerFiles
	googleweather.h
	googleweatherinterface.h
	googleweatherwidget.h
	socialqdbusplugindata.h
)

set(QTMOC_SRCS
	googleweather.h
	googleweatherinterface.h
	googleweatherwidget.h
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

add_library (googleweather SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
	googleweather
	qtviz
	${libs}
)

INSTALL(TARGETS googleweather DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES googleweather.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)

add_subdirectory(googleweather)
