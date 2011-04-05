
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
	weatherplugin.cpp
	weatherplugininterface.cpp
	
)

set(headerFiles
	weatherplugin.h
	weatherinterface.h
	weatherplugininterface.h
)

set(QTMOC_SRCS
	weatherplugin.h
	weatherinterface.h
	weatherplugininterface.h
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
	${QT_LIBRARIES}
	${QT_QTNETWORK_LIBRARIES}
   	${QT_QTTEST_LIBRARIES}


)

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)



ADD_LIBRARY(weatherplugin SHARED ${sourceFiles} ${QT_MOC_SRCS})
#ADD_LIBRARY($$qtLibraryTarget(weatherplugin) ${$$qtLibraryTarget(weatherplugin)_sources})


TARGET_LINK_LIBRARIES(
	weatherplugin
	qtviz
	${libs}
)

INSTALL(TARGETS weatherplugin DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES weatherplugin.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)


