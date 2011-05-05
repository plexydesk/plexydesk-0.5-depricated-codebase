project(plexydesk_runner)

include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/lib/
)
IF (NOT APPLE)
IF(UNIX)
INCLUDE_DIRECTORIES(
    ${X11_INCLUDE_DIR}
)
SET (extra_headers
   plexywindow.h)

SET (extra_source
   plexywindow.cpp)

SET (extra_libs
   Xcomposite
   Xdamage)

ENDIF(UNIX)
ENDIF(NOT APPLE)

########### source files ###############
SET(sourceFiles
    desktopview.cpp
    icon.cpp
    iconjob.cpp
    iconprovider.cpp
    plexypanel.cpp
    ${extra_source}
    main.cpp
)

set(headerFiles
    desktopview.h
    icon.h
    iconjob.h
    iconprovider.h
    plexypanel.h
    ${extra_headers}
)

set(QTMOC_SRCS
    desktopview.h
    icon.h
    iconjob.h
    iconprovider.h
    plexypanel.h
    ${extra_headers}
)

qt4_wrap_cpp(QT_MOC_SRCS ${QTMOC_SRCS})

set(sourceFiles
    ${sourceFiles}
    ${headerFiles}
)

IF(WIN32)
SET(GUI_TYPE WIN32)
ADD_DEFINITIONS(-DNODEFAULTLIB:library)
SET (${QT_LIBRARIES} qtmain ${QT_LIBRARIES})
ENDIF(WIN32)

IF(UNIX)
set(libs
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
   )
ELSE(UNIX)
set(libs
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    ${extra_libs}
)
ENDIF(UNIX)

########### prepare files ###############
ADD_EXECUTABLE(plexydesk ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
	plexydesk
	plexydeskcore
   plexymime
	${libs}
)

########### install files ###############
INSTALL(TARGETS plexydesk DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)

add_subdirectory(test)
