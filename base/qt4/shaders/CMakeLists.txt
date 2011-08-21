SET(sourceFiles
    shadereffect.cpp
    shadereffectbuffer.cpp
    shadereffectitem.cpp
    shadereffectsource.cpp
    scenegraph/qsggeometry.cpp
    )

SET(headerFiles
    shadereffect.h
    shadereffectbuffer.h
    shadereffectitem.h
    shadereffectsource.h
    scenegraph/qsggeometry.h
    )

SET(mocFiles
    shadereffect.h
    shadereffectitem.h
    shadereffectsource.h
    )

QT4_WRAP_CPP(QT_MOC_SRCS ${mocFiles})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    ${QT_MOC_SRCS}
    )

ADD_LIBRARY(plexyshaders SHARED ${sourceFiles})

IF(MINGW)
    SET_TARGET_PROPERTIES(plexyshaders PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(plexyshaders
    ${QT_QTSCRIPT_LIBRARY}
    ${QT_QTWEBKIT_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTSVG_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${QT_QTOPENGL_LIBRARY}
    ${QT_QTDECLARATIVE_LIBRARY}
    )

INSTALL(TARGETS plexyshaders DESTINATION ${CMAKE_INSTALL_LIBDIR})
