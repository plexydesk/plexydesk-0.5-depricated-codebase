SET(sourceFiles
	irc.cpp
)

set(headerFiles
	irc.h
)

set(QTMOC_SRCS
	irc.h
)

qt4_wrap_cpp(QT_MOC_SRCS ${QTMOC_SRCS})

set(sourceFiles
	${sourceFiles}
	${headerFiles}
)

add_library (plexyirc SHARED ${sourceFiles} ${QT_MOC_SRCS})

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTXML_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
)

set(libs
    ${QT_QTGUI_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTCORE_LIBRARY}
	${QT_QTNETWORK_LIBRARY}
    ${QT_QTSVG_LIBRARY}
)

TARGET_LINK_LIBRARIES(
    plexyirc
	${libs}
)

if(NOT WIN32)
INSTALL(TARGETS plexyirc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
elseif(WIN32)
INSTALL(TARGETS plexyirc DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
endif(NOT WIN32)
