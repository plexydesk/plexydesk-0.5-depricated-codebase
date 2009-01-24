project(videoview)

add_definitions (${QT_DEFINITIONS})

set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

SET (sourceFiles
	videoitem.cpp
	videointerface.cpp
    videoplugin.cpp
)

set(headerFiles
	videoitem.h
	videointerface.h
    videoplugin.h
)

set(QTMOC_SRCS
	videoitem.h
	videointerface.h
        videoplugin.h
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

INCLUDE_DIRECTORIES(
	${QT_INCLUDES}
	${QT_QTOPENGL_INCLUDE_DIR}
	${QT_QTNETWORK_INCLUDE_DIR}
	${QT_QTTEST_INCLUDE_DIR}
)

add_library (videoview SHARED ${sourceFiles} ${QT_MOC_SRCS})

target_link_libraries(
	videoview
	qtviz
	${libs}
)

INSTALL(TARGETS videoview DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES videoview.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)

add_subdirectory(img)

