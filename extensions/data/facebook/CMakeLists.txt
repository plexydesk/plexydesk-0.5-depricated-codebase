SET (sourceFiles
     facebooksession.cpp
     facebookdatainterface.cpp
     facebookuserinfo.cpp
    )

SET(headerFiles facebooksession.h facebookdatainterface.h facebookuserinfo.h
    )

SET (QTMOC_SRCS facebooksession.h facebookdatainterface.h facebookuserinfo.h
    )

QT5_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
    )

SET(libs
    jsoncpp
    )

ADD_LIBRARY(facebookengine SHARED ${sourceFiles} ${QT_MOC_SRCS})

IF(MINGW)
    SET_TARGET_PROPERTIES(facebookengine PROPERTIES
        IMPORT_SUFFIX ".lib"
        IMPORT_PREFIX ""
        PREFIX ""
        )
ENDIF(MINGW)

TARGET_LINK_LIBRARIES(facebookengine
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )

QT5_USE_MODULES(facebookengine Widgets Xml OpenGL Core WebKitWidgets Svg Declarative)

INSTALL(TARGETS facebookengine DESTINATION ${CMAKE_INSTALL_LIBDIR}/plexyext)
INSTALL(FILES facebook.desktop DESTINATION share/plexy/ext/groups)
