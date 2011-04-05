INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
	${QT_INCLUDES}
	${QT_QTOPENGL_INCLUDE_DIR}
	${QT_QTNETWORK_INCLUDE_DIR}
	${QT_QTTEST_INCLUDE_DIR}
   ${QIMAGE_BLITZ_INCLUDE_DIR}/qimageblitz
)

QT4_WRAP_UI(QT_LOGIN_UI login.ui)

SET (sourceFiles 
     ${QT_LOGIN_UI} 
     login.cpp 
     welcomewidget.cpp 
     frameitem.cpp  
     plugin.cpp
     welcomeinterface.cpp 
     welcomeitem.cpp )

SET (headerFiles 
   login.h
   welcomewidget.h
   frameitem.h
   plugin.h
   welcomeinterface.h
   welcomeitem.h)

SET(QTMOC_SRCS login.h
   welcomewidget.h 
   frameitem.h  
   plugin.h  
   welcomeinterface.h 
   welcomeitem.h)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

qt4_wrap_cpp(QT_MOC_SRCS ${QTMOC_SRCS})

set(sourceFiles
	${sourceFiles}
	${headerFiles}
)

set(libs ${QT_QTGUI_LIBRARY}
	${OPENGL_LIBRARIES}
	${QT_QTCORE_LIBRARY}
	${QT_QTOPENGL_LIBRARY}
   ${QIMAGE_BLITZ_LIBRARY}
)


add_library (plexywelcome SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
	plexywelcome
   plexydeskcore
	${libs}
)

INSTALL(TARGETS plexywelcome DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext/)
install(FILES welcome.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)

#add_subdirectory(welcome)
