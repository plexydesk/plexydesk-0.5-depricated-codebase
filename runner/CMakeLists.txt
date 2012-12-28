# Check if we use any Debug in the final release and if so compile the tests
IF(CMAKE_BUILD_TYPE MATCHES ".*Deb.*")
#    ADD_SUBDIRECTORY(test)
ENDIF(CMAKE_BUILD_TYPE MATCHES ".*Deb.*")


IF (NOT APPLE)
    IF(UNIX)
        INCLUDE_DIRECTORIES(
            ${X11_INCLUDE_DIR}
            )

        SET (extra_headers)

        SET (extra_source)

        SET (extra_libs
             Xcomposite
             Xdamage
             )
    ENDIF(UNIX)
    IF(WIN32)
        INCLUDE_DIRECTORIES(
            ${CMAKE_SOURCE_DIR}/dist/windows
            ${CMAKE_BINARY_DIR}/dist/windows
            )
        SET(resourceFiles
                ${CMAKE_BINARY_DIR}/dist/windows/${APPLICATION_EXE_NAME}_windres.rc
                )
    ENDIF(WIN32)
ENDIF(NOT APPLE)

########### Source files ###############
SET(sourceFiles
    desktopbaseui.cpp
    ${extra_source}
    )


if(APPLE)
  SET(mainFiles main.mm)
ENDIF(APPLE)

IF (NOT APPLE)
    SET(mainFiles main.cpp)
ENDIF(NOT APPLE)

SET(headerFiles
    desktopbaseui.h
    ${extra_headers}
    )

SET(QTMOC_SRCS
    desktopbaseui.h
    ${extra_headers}
    )

QT5_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    ${mainFiles}
    )

IF(WIN32)
    SET(GUI_TYPE WIN32)
    ADD_DEFINITIONS(-DNODEFAULTLIB:library)
    SET(libs
        ${QT_QTGUI_LIBRARY}
        ${QT_QTCORE_LIBRARY}
        ${QT_QTOPENGL_LIBRARY}
        ${QT_QTMAIN_LIBRARY}
        )
ENDIF(WIN32)

IF(UNIX)
    SET(libs
        ${extra_libs}
        )
ENDIF(UNIX)

#Locate cococa framework on mac
IF (APPLE)
   INCLUDE_DIRECTORIES ( /System/Library/Frameworks )
   FIND_LIBRARY(COCOA_LIBRARY Cocoa)
   FIND_LIBRARY(GLUT_LIBRARY GLUT)
   FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
   MARK_AS_ADVANCED (COCOA_LIBRARY
                     GLUT_LIBRARY
                     OpenGL_LIBRARY)
   SET(MAC_LIBS ${COCOA_LIBRARY} ${GLUT_LIBRARY} ${OpenGL_LIBRARY})
ENDIF(APPLE)

########### Prepare files ###############
CMAKE_POLICY(SET CMP0003 NEW)

IF (APPLE)
set (APP_TYPE MACOSX_BUNDLE)
ENDIF(APPLE)
ADD_EXECUTABLE(${APPLICATION_EXE_NAME} ${APP_TYPE} ${WIN_APPLICATION} ${sourceFiles} ${resourceFiles} ${QT_MOC_SRCS})

IF(MSVC)
    MSVC_SET_PROPER_CONSOLE_HANDLING(${APPLICATION_EXE_NAME})
ENDIF(MSVC)

TARGET_LINK_LIBRARIES(${APPLICATION_EXE_NAME}
    ${PLEXY_UI_CORE_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${MAC_LIBS}
    )

QT5_USE_MODULES(${APPLICATION_EXE_NAME} Widgets Xml OpenGL Core WebKitWidgets Svg Declarative)

########### Install files ###############
INSTALL(TARGETS plexydesk DESTINATION bin)

IF(UNIX)
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/plexydesk.desktop DESTINATION share/applications)
ENDIF(UNIX)
