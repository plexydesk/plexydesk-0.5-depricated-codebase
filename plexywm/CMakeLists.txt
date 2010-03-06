project(plexydesk)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
   ${QT_INCLUDES}
   ${QT_QTOPENGL_INCLUDE_DIR}
   ${QT_QTNETWORK_INCLUDE_DIR}
   ${QT_QTTEST_INCLUDE_DIR}
   ${X11_INCLUDE_DIR}
 )

include_directories(
   ${CMAKE_SOURCE_DIR}/lib/
   ${CMAKE_SOURCE_DIR}/base/qt4/
   )
########### source files ###############
SET(sourceFiles 
	plexywindow.cpp
	main.cpp
	windowmanager.cpp
)

set(QTMOC_SRCS plexywindow.h windowmanager.h)

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
ADD_EXECUTABLE(plexydeskwm ${sourceFiles} ${QT_MOC_SRCS})
#KDE4_ADD_PLUGIN(plexydeskwm ${raptor_plasmoid_PART_SRCS})

TARGET_LINK_LIBRARIES(
	plexydeskwm 
	plexydeskcore
        Xcomposite
        Xdamage
	${libs}
)


########### install files ###############
INSTALL(TARGETS plexydeskwm DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
#INSTALL( FILES raptor.desktop  DESTINATION  ${DATA_INSTALL_DIR}/kicker/applets )
