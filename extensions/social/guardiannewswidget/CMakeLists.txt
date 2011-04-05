
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
	guardiannews.cpp
	guardiannewsinterface.cpp 
	guardiannewswidget.cpp
	socialqdbusplugindata.cpp
)

set(headerFiles
	guardiannews.h
	guardiannewsinterface.h 
	guardiannewswidget.h
	socialqdbusplugindata.h
)

set(QTMOC_SRCS
	guardiannews.h
	guardiannewsinterface.h 
	guardiannewswidget.h
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

add_library (guardiannews SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
	guardiannews
	qtviz
	${libs}
)

INSTALL(TARGETS guardiannews DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES guardiannews.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)

add_subdirectory(guardiannews)

