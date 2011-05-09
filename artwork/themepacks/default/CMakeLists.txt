PROJECT(PlexyDesktop)

FILE(GLOB PNGFILES resources/*.png )
FILE(GLOB CFG main.cfg)
SET(WEATHER weather/weather.qml)

INSTALL(FILES ${PNGFILES}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/resources/)
INSTALL(FILES ${CFG}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default)
INSTALL(FILES ${WEATHER}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/weather/)
