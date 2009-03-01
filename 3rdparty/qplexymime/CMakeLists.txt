
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	${QT_INCLUDES}
	${QT_QTXMLPATTERNS_INCLUDE_DIR}
)

SET(resourceFiles
	qplexymime.qrc
)

QT4_ADD_RESOURCES(QT_RCC_SRCS ${resourceFiles})

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
	${QT_QTXMLPATTERNS_LIBRARY}
)

add_library (plexymime SHARED ${sourceFiles} ${QT_MOC_SRCS} ${QT_RCC_SRCS})

TARGET_LINK_LIBRARIES(
	plexymime
	${libs}
)

INSTALL(TARGETS plexymime DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

add_subdirectory(tests)
