PROJECT(PlexyDesktop)

FILE(GLOB PNGFILES resources/*.png )
FILE(GLOB CFG main.cfg)

INSTALL(FILES ${PNGFILES}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default/resources/)
INSTALL(FILES ${CFG}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/themepack/default)
