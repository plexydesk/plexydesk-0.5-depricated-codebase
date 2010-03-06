
set(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	   ${QT_INCLUDES}
	   ${QT_QTOPENGL_INCLUDE_DIR}
	   ${QT_QTNETWORK_INCLUDE_DIR}
	   ${QT_QTTEST_INCLUDE_DIR}
      ${OPENCV_INCLUDE_DIR}
)

SET (sourceFiles 	
    webcam.cpp
    webcaminterface.cpp
)

set(headerFiles
    webcam.h
    webcaminterface.h
)

SET (QTMOC_SRCS
    webcam.h
    webcaminterface.h
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
	${QT_QTNETWORK_LIBRARY}
	${QT_QTOPENGL_LIBRARY}
   ${OPENCV_LIBRARIES}
)

add_library (webcamengine SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
	webcamengine
	plexydeskcore
	${libs}
)

INSTALL(TARGETS webcamengine DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES webcam.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)
