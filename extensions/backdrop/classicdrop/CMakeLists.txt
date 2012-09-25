SET(sourceFiles
    backdrop.cpp
    classicinterface.cpp
    classicbackgroundrender.cpp
    )

SET(headerFiles
    backdrop.h
    classicinterface.h
    classicbackgroundrender.h
    )

SET(QTMOC_SRCS
    backdrop.h
    classicinterface.h
    classicbackgroundrender.h
    )

QT4_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )

SET(libs
    ${QT_QTGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    )

ADD_LIBRARY(classicbackdrop SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(classicbackdrop PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(classicbackdrop
    ${PLEXY_UI_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS classicbackdrop DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES classic.desktop DESTINATION share/plexy/ext/groups)
