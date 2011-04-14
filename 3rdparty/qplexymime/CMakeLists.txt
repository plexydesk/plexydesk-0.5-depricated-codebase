add_subdirectory(tests)

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
        ${QT_QTCORE_LIBRARY}
	${QT_QTXMLPATTERNS_LIBRARY}
)

add_library (plexymime SHARED ${sourceFiles} ${QT_MOC_SRCS} ${QT_RCC_SRCS})

TARGET_LINK_LIBRARIES(
	plexymime
	${libs}
)

IF (NOT WIN32)
    INSTALL(TARGETS plexymime DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
ELSEIF(WIN32)
    INSTALL(TARGETS plexymime DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
ENDIF(NOT WIN32)

FILE(GLOB XML_FILES *.xml)

INSTALL(FILES ${XML_FILES}  DESTINATION ${CMAKE_INSTALL_PREFIX}/mime/)
