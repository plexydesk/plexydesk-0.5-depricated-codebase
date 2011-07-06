INCLUDE_DIRECTORIES(
	   ${QT_INCLUDES}
	   ${QT_QTOPENGL_INCLUDE_DIR}
	   ${QT_QTNETWORK_INCLUDE_DIR}
	   ${QT_QTTEST_INCLUDE_DIR}
	   )

SET (sourceFiles
    utube.cpp
    youtubedatainterface.cpp
    )

SET(headerFiles
    utube.h
    youtubedatainterface.h
    )

SET(QTMOC_SRCS
    utube.h
    youtubedatainterface.h
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
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    )

ADD_LIBRARY(utubeengine SHARED ${sourceFiles} ${QT_MOC_SRCS})

TARGET_LINK_LIBRARIES(
    utubeengine
    ${PLEXY_CORE_LIBRARY}
    ${libs}
    )

INSTALL(TARGETS utubeengine DESTINATION lib/plexyext)
INSTALL(FILES utube.desktop DESTINATION share/plexy/ext/groups)
