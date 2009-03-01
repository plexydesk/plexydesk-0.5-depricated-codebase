include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	${QT_INCLUDES}
	${QT_QTXMLPATTERNS_INCLUDE_DIR}
)

SET(sourceFiles
	test.cpp
)

set(sourceFiles
	${sourceFiles}
)

set(libs
	${QT_QTCORE_LIBRARY}
	${QT_QTXMLPATTERNS_LIBRARY}
)

ADD_EXECUTABLE(plexymimetest ${sourceFiles})

TARGET_LINK_LIBRARIES(
	plexymimetest
	plexymime
	${libs}
)

INSTALL(TARGETS plexymimetest DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
