FILE(GLOB PNGFILES resources/*.png )
FILE(GLOB JPEGFILES resources/*.jpg )
FILE(GLOB SVGFILES resources/*.svg )
FILE(GLOB CFG main.cfg)
SET(RES_DEST share/plexy/themepack/default/resources/)
SET(WEATHER weather/weather.qml)

# Photo CMAKE entries
SET(PHOTO photo/photoElement.qml)
SET(GLOB PHOTO_IMGS photo/photoElement_images/*.*)

#Photo Frame entries

SET(PHOTO_FRAME photoframe/photoframe.qml)
FILE(GLOB PHOTO_FRAME_IMGS photoframe/frameimages/*.png)


# Folderview CMAKE entries
SET(FOLDERVIEW folderview/folderview.qml)
SET(FOLDERVIEW_RES_QML folderview/resources/Icon.qml)
SET(FOLDERVIEW_RES_JS folderview/resources/Scripts.js)
FILE(GLOB FOLDERVIEW_IMGS folderview/resources/images/*.png)

# DigitalClock CMAKE entries
SET(DIGITALCLOCK digitalclock/digitalClock.qml)
FILE(GLOB DIGITALCLOCK_IMGS digitalclock/images/*.png)

# Welcome widget CMAKE entries
SET(WELCOMEQML welcome/welcome.qml)
SET(WELCOMESCRIPTS welcome/scripts/Scripts.js)
FILE(GLOB WELCOMEIMGS welcome/images/*.png)

# PhotoViewer CMAKE entries
SET(PHOTOVIEWER photoviewer/photoviewer.qml)
FILE(GLOB PHOTOVIEWER_COM photoviewer/PhotoViewerCore/*.qml)
FILE(GLOB PHOTOVIEWER_IMG photoviewer/PhotoViewerCore/images/*.png)
SET(PHOTOVIEWER_SCR photoviewer/PhotoViewerCore/script/script.js)


# *** Check files in the final QT resources folder are unique - UNIX PLATFORMS ***
# PLEASE ADD TO $UNIQUE_RC_FILES BELOW ALL RESOURCE FILES THAT GO TO ${RES_DEST}
IF (UNIX)
    MESSAGE(STATUS "Checking for name clashes in the QT RESOURCE folder...")
    SET(UNIQUE_RC_FILES ${PNGFILES}
                        ${JPEGFILES}
                        ${SVGFILES}
                        ${PHOTO_IMGS}
                        ${FOLDERVIEW_IMGS}
                        ${DIGITALCLOCK_IMGS}
                        ${WELCOMEIMGS}
                        ${PHOTOVIEWER_IMG}
                        ${PHOTO_FRAME_IMGS}
                        )
    EXECUTE_PROCESS(COMMAND ${CMAKE_SOURCE_DIR}/cmakemodules/get_duplicate_files.sh "${UNIQUE_RC_FILES}"
                    OUTPUT_VARIABLE OUT_UNIQ
                    )
    IF(NOT ${OUT_UNIQ} STREQUAL "")
        MESSAGE(FATAL_ERROR "There are name clashes in the QT RESOURCE folder:\n${OUT_UNIQ}")
    ENDIF(NOT ${OUT_UNIQ} STREQUAL "")
ENDIF (UNIX)

INSTALL(FILES ${PNGFILES}  DESTINATION ${RES_DEST})
INSTALL(FILES ${JPEGFILES}  DESTINATION ${RES_DEST})
INSTALL(FILES ${SVGFILES}  DESTINATION ${RES_DEST})
INSTALL(FILES ${CFG}  DESTINATION share/plexy/themepack/default)
INSTALL(FILES ${WEATHER}  DESTINATION share/plexy/themepack/default/weather)

INSTALL(FILES ${PHOTO}  DESTINATION share/plexy/themepack/default/photo)
INSTALL(FILES ${PHOTO_IMGS}  DESTINATION ${RES_DEST})

INSTALL(FILES ${PHOTO_FRAME}  DESTINATION share/plexy/themepack/default/photoframe)
INSTALL(FILES ${PHOTO_FRAME_IMGS}  DESTINATION ${RES_DEST})


INSTALL(FILES ${FOLDERVIEW} DESTINATION share/plexy/themepack/default/folderview)
INSTALL(FILES ${FOLDERVIEW_RES_QML} DESTINATION share/plexy/themepack/default/folderview/resources)
INSTALL(FILES ${FOLDERVIEW_RES_JS} DESTINATION share/plexy/themepack/default/folderview/resources)
INSTALL(FILES ${FOLDERVIEW_IMGS} DESTINATION ${RES_DEST})

INSTALL(FILES ${DIGITALCLOCK} DESTINATION share/plexy/themepack/default/digitalclock)
INSTALL(FILES ${DIGITALCLOCK_IMGS} DESTINATION ${RES_DEST})

INSTALL(FILES ${WELCOMEQML} DESTINATION share/plexy/themepack/default/welcome)
INSTALL(FILES ${WELCOMESCRIPTS} DESTINATION share/plexy/themepack/default/welcome/scripts)
INSTALL(FILES ${WELCOMEIMGS} DESTINATION ${RES_DEST})

INSTALL(FILES ${PHOTOVIEWER} DESTINATION share/plexy/themepack/default/photoviewer)
INSTALL(FILES ${PHOTOVIEWER_COM} DESTINATION share/plexy/themepack/default/photoviewer/PhotoViewerCore)
INSTALL(FILES ${PHOTOVIEWER_IMG} DESTINATION ${RES_DEST})
INSTALL(FILES ${PHOTOVIEWER_SCR} DESTINATION share/plexy/themepack/default/photoviewer/PhotoViewerCore/script)

#wallpaper item
FILE (GLOB WALL_QML_ITEMS backdrop/*.qml)

INSTALL(FILES ${WALL_QML_ITEMS} DESTINATION share/plexy/themepack/default/backdrop)

