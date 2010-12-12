project(plexyfrens)
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(
   ${QT_INCLUDES}
   ${QT_QTOPENGL_INCLUDE_DIR}
   ${QT_QTNETWORK_INCLUDE_DIR}
   ${QT_QTTEST_INCLUDE_DIR}
   ${X11_INCLUDE_DIR}
 )

include_directories(${CMAKE_SOURCE_DIR}/lib/)
########### source files ###############
SET(sourceFiles
   desktopview.cpp
   main.cpp
)

set(QTMOC_SRCS desktopview.h)

qt4_wrap_cpp(QT_MOC_SRCS ${QTMOC_SRCS})

IF(WIN32)
SET(GUI_TYPE WIN32)
ADD_DEFINITIONS(-DNODEFAULTLIB:library)
SET (${QT_LIBRARIES} qtmain ${QT_LIBRARIES})
ENDIF(WIN32)

set(libs
	${QT_QTGUI_LIBRARY}
	${QT_QTCORE_LIBRARY}
   ${QT_QTOPENGL_LIBRARY}
	${QT_AND_KDECORE_LIBS}
)

########### prepare files ###############
ADD_EXECUTABLE(plexyfrens ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
	plexyfrens 
	plexydeskcore
	${libs}
)


########### install files ###############
INSTALL(TARGETS plexyfrens DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
