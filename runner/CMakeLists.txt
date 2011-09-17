# Check if we use any Debug in the final release and if so compile the tests
IF(CMAKE_BUILD_TYPE MATCHES ".*Deb.*")
    ADD_SUBDIRECTORY(test)
ENDIF(CMAKE_BUILD_TYPE MATCHES ".*Deb.*")


IF (NOT APPLE)
    IF(UNIX)
        INCLUDE_DIRECTORIES(
            ${X11_INCLUDE_DIR}
            )

        SET (extra_headers
             plexywindow.h
             )

        SET (extra_source
             plexywindow.cpp
             )

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
    desktopview.cpp
    icon.cpp
    iconjob.cpp
    iconprovider.cpp
#    plexyeventhandler.cpp
    desktopbaseui.cpp
    plexypanel.cpp
    plexytray.cpp
    ${extra_source}
    main.cpp
    )

SET(headerFiles
    desktopview.h
    icon.h
    iconjob.h
    iconprovider.h
#    plexyeventhandler.h
    desktopbaseui.h
    plexypanel.h
    plexytray.h
    ${extra_headers}
    )

SET(QTMOC_SRCS
    desktopview.h
    icon.h
    iconjob.h
    iconprovider.h
#    plexyeventhandler.h
    desktopbaseui.h
    plexypanel.h
    plexytray.h
    ${extra_headers}
    )

QT4_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
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
        ${QT_QTGUI_LIBRARY}
        ${QT_QTCORE_LIBRARY}
        ${QT_QTOPENGL_LIBRARY}
        ${extra_libs}
        )
ENDIF(UNIX)

########### Prepare files ###############
CMAKE_POLICY(SET CMP0003 NEW)

ADD_EXECUTABLE(${APPLICATION_EXE_NAME} MACOSX_BUNDLE ${WIN_APPLICATION} ${sourceFiles} ${resourceFiles} ${QT_MOC_SRCS})

IF(MSVC)
    MSVC_SET_PROPER_CONSOLE_HANDLING(${APPLICATION_EXE_NAME})
ENDIF(MSVC)

TARGET_LINK_LIBRARIES(${APPLICATION_EXE_NAME}
    ${PLEXY_CORE_LIBRARY}
    plexymime
    ${libs}
    )

########### Install files ###############
INSTALL(TARGETS plexydesk DESTINATION bin)

IF(UNIX)
    INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/plexydesk.desktop DESTINATION share/applications)
ENDIF(UNIX)
