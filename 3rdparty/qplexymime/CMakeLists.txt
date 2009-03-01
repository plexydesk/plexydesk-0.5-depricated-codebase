
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	${QT_INCLUDES}
	${QT_XMLPATTERNS_INCLUDE_DIR}
)

SET(sourceFiles
	qplexymime.cpp
)

set(headerFiles
	qplexymime.h
)

set(QTMOC_SRCS
	qplexymime.h
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
	${QT_XMLPATTERNS_LIBRARY}
)

add_library (plexymime SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
	plexymime
	${libs}
)

INSTALL(TARGETS plexymime DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

add_subdirectory(tests)
