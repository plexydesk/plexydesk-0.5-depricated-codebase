ADD_SUBDIRECTORY(test)

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/lib
)

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
            ${CMAKE_SOURCE_DIR}/artwork/windows
            ${CMAKE_BINARY_DIR}/artwork/windows
            )
        IF(MINGW)
            SET(resourceFiles
                ${CMAKE_BINARY_DIR}/artwork/windows/${APPLICATION_EXE_NAME}_rc.o
                )
        ELSE(MINGW)
            SET(resourceFiles
                ${CMAKE_BINARY_DIR}/artwork/windows/${APPLICATION_EXE_NAME}_windres.rc
                )
        ENDIF(MINGW)
    ENDIF(WIN32)

ENDIF(NOT APPLE)

########### Source files ###############
SET(sourceFiles
    desktopview.cpp
    icon.cpp
    iconjob.cpp
    iconprovider.cpp
    plexypanel.cpp
    themepackloader.cpp
    ${extra_source}
    main.cpp
    )

SET(headerFiles
    desktopview.h
    icon.h
    iconjob.h
    iconprovider.h
    plexypanel.h
    themepackloader.h
    ${extra_headers}
    )

SET(QTMOC_SRCS
    desktopview.h
    icon.h
    iconjob.h
    iconprovider.h
    plexypanel.h
    themepackloader.h
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
    SET(${QT_LIBRARIES} qtmain ${QT_LIBRARIES})
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

ADD_EXECUTABLE(${APPLICATION_EXE_NAME} ${WIN_APPLICATION} ${sourceFiles} ${resourceFiles} ${QT_MOC_SRCS})

IF(MSVC)
    MSVC_SET_PROPER_CONSOLE_HANDLING(${APPLICATION_EXE_NAME})
ENDIF(MSVC)

TARGET_LINK_LIBRARIES(
    ${APPLICATION_EXE_NAME}
    ${PLEXY_CORE_LIBRARY}
    plexymime
    ${libs}
    )

########### Install files ###############
INSTALL(TARGETS plexydesk DESTINATION bin)
