
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
	abstractplugininterface.cpp
	backdropinterface.cpp
	backdropplugin.cpp
	baseplugin.cpp
	baserender.cpp
	canvas.cpp
	dataplugin.cpp
	desktopview.cpp
	desktopwidget.cpp
	extensionfactory.cpp
	fakemime.cpp
	frameitem.cpp
	navigator.cpp
	netwm.cpp
	plexyconfig.cpp
	pluginloader.cpp
	viewlayer.cpp
	widgetplugin.cpp
)

set(headerFiles
	abstractplugininterface.h
	backdropinterface.h
	backdropplugin.h
	baseplugin.h
	baserender.h
	canvas.h
	datainterface.h
	dataplugin.h
	desktopview.h
	desktopwidget.h
	extensionfactory.h
	fakemime.h
	frameitem.h
	navigator.h
	netwm.h
	netwm_def.h
	netwm_p.h
	plexyconfig.h
	pluginloader.h
	viewlayer.h
	widgetinterface.h
	widgetplugin.h
)

set(QTMOC_SRCS
	abstractplugininterface.h
	backdropplugin.h
	baseplugin.h
	baserender.h
	canvas.h
	datainterface.h
	dataplugin.h
	desktopview.h
	desktopwidget.h
	navigator.h
	plexyconfig.h
	pluginloader.h
	viewlayer.h
	widgetplugin.h
	widgetinterface.h
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
    ${QT_QTSVG_LIBRARY}
)

add_library(qtviz SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
    qtviz
    qimageblitz
	${libs}
)

if (NOT WIN32)
    INSTALL(TARGETS qtviz DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
elseif(WIN32)
    INSTALL(TARGETS qtviz DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
endif(NOT WIN32)
