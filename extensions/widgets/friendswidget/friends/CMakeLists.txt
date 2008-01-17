PROJECT(PlexyDesktop)

FILE(GLOB PNGFILES *.png )

INSTALL(FILES ${PNGFILES}  DESTINATION /usr/share/plexy/skins/default/friendswidget/)
