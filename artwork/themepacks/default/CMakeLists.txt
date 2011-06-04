PROJECT(PlexyDesktop)

FILE(GLOB PNGFILES resources/*.png )
FILE(GLOB JPEGFILES resources/*.jpg ) 
FILE(GLOB CFG main.cfg)
SET(WEATHER weather/weather.qml)

#photo CMAKE entries
SET(PHOTO photo/photoElement.qml)
SET(PHOTO_IMGS photo/photoElement_images/background.png)
SET(PHOTO_IMG_1 photo/photoElement_images/pasted_layer.png)
#Folderview CMAKE entries
SET(FOLDERVIEW folderview/folderview.qml)
SET(FOLDERVIEW_RES_QML folderview/resources/Icon.qml)
SET(FOLDERVIEW_RES_JS folderview/resources/Scripts.js)
FILE(GLOB FOLDERVIEW_IMGS folderview/resources/images/*.png)
#DigitalClock CMAKE entries
SET(DIGITALCLOCK digitalclock/digitalClock.qml)
FILE(GLOB DIGITALCLOCK_IMGS digitalclock/images/*.png)
#Welcome widget CMAKE entries
SET(WELCOMEQML welcome/welcome.qml)
SET(WELCOMESCRIPTS welcome/scripts/Scripts.js)
FILE(GLOB WELCOMEIMGS welcome/images/*.png)
#PhotoViewer CMAKE entries
SET(PHOTOVIEWER photoviewer/photoviewer.qml)
FILE(GLOB PHOTOVIEWER_COM photoviewer/PhotoViewerCore/*.qml)
FILE(GLOB PHOTOVIEWER_IMG photoviewer/PhotoViewerCore/images/*.png)
SET(PHOTOVIEWER_SCR photoviewer/PhotoViewerCore/script/script.js)

INSTALL(FILES ${PNGFILES}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/resources/)
INSTALL(FILES ${JPEGFILES}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/resources/)
INSTALL(FILES ${CFG}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default)
INSTALL(FILES ${WEATHER}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/weather/)
INSTALL(FILES ${PHOTO}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/photo/)
INSTALL(FILES ${PHOTO_IMGS}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/photo/photoElement_images/)
INSTALL(FILES ${PHOTO_IMG_1}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/photo/photoElement_images/)

INSTALL(FILES ${FOLDERVIEW} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/folderview/)
INSTALL(FILES ${FOLDERVIEW_RES_QML} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/folderview/resources)
INSTALL(FILES ${FOLDERVIEW_RES_JS} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/folderview/resources)
INSTALL(FILES ${FOLDERVIEW_IMGS} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/folderview/resources/images)

INSTALL(FILES ${DIGITALCLOCK} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/digitalclock/)
INSTALL(FILES ${DIGITALCLOCK} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/digitalclock/)
INSTALL(FILES ${DIGITALCLOCK_IMGS} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/digitalclock/images)

INSTALL(FILES ${WELCOMEQML} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/welcome/)
INSTALL(FILES ${WELCOMESCRIPTS} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/welcome/scripts/)
INSTALL(FILES ${WELCOMEIMGS} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/welcome/images/)

INSTALL(FILES ${PHOTOVIEWER} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/photoviewer/)
INSTALL(FILES ${PHOTOVIEWER_COM} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/photoviewer/PhotoViewerCore/)
INSTALL(FILES ${PHOTOVIEWER_IMG} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/photoviewer/PhotoViewerCore/images)
INSTALL(FILES ${PHOTOVIEWER_SCR} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/photoviewer/PhotoViewerCore/script)
