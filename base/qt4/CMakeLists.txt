
set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
)


SET(sourceFiles icon.cpp
	abstractplugininterface.cpp
	backdropinterface.cpp
	canvas.cpp
   baserender.cpp
	desktopwidget.cpp
	extensionfactory.cpp
	fakemime.cpp
	frameitem.cpp
	navigator.cpp
	netwm.cpp
	plexyconfig.cpp
	pluginloader.cpp
	viewlayer.cpp
   iconprovider.cpp
   pendingjob.cpp
   iconjob.cpp
   listview.cpp
   httpjobhandler.cpp
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
	icon.h
   iconprovider.h
   pendingjob.h
   iconjob.h
   listview.h
   httpjobhandler.h
)

set(QTMOC_SRCS
	abstractplugininterface.h
	backdropplugin.h
	baseplugin.h
	baserender.h
	canvas.h
	datainterface.h
	dataplugin.h
	desktopwidget.h
	navigator.h
	plexyconfig.h
	pluginloader.h
	viewlayer.h
	widgetplugin.h
	widgetinterface.h
	icon.h
   iconprovider.h
   pendingjob.h
   iconjob.h
   listview.h
   httpjobhandler.h
)

#for now only dbus works only under linux
IF (UNIX)
QT4_ADD_DBUS_ADAPTOR(dbus_SRCS org.plexydesk.Config.xml
                        plexyconfig.h PlexyDesk::Config)
message(${dbus_SRCS})
ENDIF(UNIX)


qt4_wrap_cpp(QT_MOC_SRCS ${QTMOC_SRCS})

set(sourceFiles
	${sourceFiles}
	${headerFiles}
   ${dbus_SRCS}
)

set(libs
    ${QT_QTGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    ${QT_QTSVG_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
)

add_library(plexydeskcore SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
    plexydeskcore
    qimageblitz
    plexymime
	${libs}
)
if (NOT WIN32)
    INSTALL(TARGETS plexydeskcore DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
elseif(WIN32)
    INSTALL(TARGETS plexydeskcore DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
endif(NOT WIN32)
