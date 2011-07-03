FILE(GLOB PNGFILES *.svg *.png )
FILE(GLOB GIFFILES *.gif )

INSTALL(FILES ${PNGFILES}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/skins/default/guardiannews)
INSTALL(FILES ${GIFFILES}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/skins/default/guardiannews)
