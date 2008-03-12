PROJECT(PlexyDesktop)

FILE(GLOB PNGFILES *.svg )

INSTALL(FILES ${PNGFILES}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/skins/default/welcome/)
