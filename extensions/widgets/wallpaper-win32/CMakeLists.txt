
SET(QT_USE_OPENGL TRUE)
# -> this will cause cmake to include and link against the OpenGL module
INCLUDE(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
    ${QT_INCLUDES}
    ${QT_QTOPENGL_INCLUDE_DIR}
    ${QT_QTNETWORK_INCLUDE_DIR}
)

SET(sourceFiles
    wallpaper.cpp
    wallpaperwidget.cpp
    wallpaperinterface.cpp
)
    
SET(headerFiles
    wallpaper.h
    wallpaperwidget.h
    wallpaperinterface.h
    config.h
)
    
SOURCE_GROUP(source
    FILES
    ${sourceFile}
)
    
SOURCE_GROUP(headers
    FILES
    ${headerFiles}
)

SET (sourceFiles
    ${sourceFiles}
    ${headerFiles}
)

SET(QTMOC_SRCS
    wallpaper.h
    wallpaperwidget.h
    wallpaperinterface.h
)

QT4_WRAP_CPP(QT_MOC_SRCS ${QTMOC_SRCS})

SET(sourceFiles
    ${sourceFiles}
    ${headerFiles}
)

ADD_LIBRARY(wallpaperchange SHARED ${sourceFiles} ${QT_MOC_SRCS})

SET(libs
    ${QT_QTGUI_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
)

TARGET_LINK_LIBRARIES(
    wallpaperchange
    plexydeskcore
#    qtviz
    ${libs}
)

INSTALL(TARGETS wallpaperchange DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plexyext)
INSTALL(FILES wallpaperchange.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/ext/groups/)
