
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
	guardianplugin.cpp
	guardianplugininterface.cpp
	
)

set(headerFiles
	guardianplugin.h
	guardianinterface.h
	guardianplugininterface.h
)

set(QTMOC_SRCS
	guardianplugin.h
	guardianinterface.h
	guardianplugininterface.h
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



ADD_LIBRARY(guardianplugin SHARED ${sourceFiles} ${QT_MOC_SRCS})
#ADD_LIBRARY($$qtLibraryTarget(guardianplugin) ${$$qtLibraryTarget(guardianplugin)_sources})


TARGET_LINK_LIBRARIES(
	guardianplugin
	qtviz
	${libs}
)

INSTALL(TARGETS guardianplugin DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES guardianplugin.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)


