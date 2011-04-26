INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTNETWORK_INCLUDE_DIR}
    ${QT_QTXML_INCLUDE_DIR}
    ${QT_QTTEST_INCLUDE_DIR}
)

SET (sourceFiles
    freedesktopmime.cpp
)

SET(headerFiles
    freedesktopmime.h
    config.h
)

SET(MOC_SRCS
    freedesktopmime.h
)

qt4_wrap_cpp(QT_MOC_SRCS ${MOC_SRCS})

SET(sourceFiles
   ${sourceFiles}
   ${headerFiles}
)

SET(libs
    ${QT_QTGUI_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTSVG_LIBRARY}
)

add_library (mimetype SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(mimetype PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
    )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(
    mimetype
    ${libs}
)

if(NOT WIN32)
INSTALL(TARGETS mimetype DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
elseif(WIN32)
INSTALL(TARGETS mimetype DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
endif(NOT WIN32)
