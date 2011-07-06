FILE(GLOB PNGFILES *.svg *.png )
FILE(GLOB GIFFILES *.gif )

INSTALL(FILES ${PNGFILES}  DESTINATION share/plexy/skins/default/guardiannews)
INSTALL(FILES ${GIFFILES}  DESTINATION share/plexy/skins/default/guardiannews)
