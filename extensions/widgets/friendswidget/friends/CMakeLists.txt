PROJECT(PlexyDesktop)

FILE(GLOB PNGFILES *.png )

INSTALL(FILES ${PNGFILES}  DESTINATION ${CMAKE_INSTALL_PREFIX}/theme/skins/default/widget/friendswidget/)
