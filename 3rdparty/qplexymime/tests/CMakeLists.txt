include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	${QT_INCLUDES}
	${QT_XMLPATTERNS_INCLUDE_DIR}
)

SET(sourceFiles
	test.cpp
)

set(headerFiles
)

set(QTMOC_SRCS
)

qt4_wrap_cpp(QT_MOC_SRCS ${QTMOC_SRCS})

set(sourceFiles
	${sourceFiles}
	${headerFiles}
)

set(libs
	${QT_QTCORE_LIBRARY}
	${QT_XMLPATTERNS_LIBRARY}
)

ADD_EXECUTABLE(plexymimetest ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
	plexymimetest
	plexymime
	${libs}
)

INSTALL(TARGETS plexymimetest DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
