PROJECT(PlexyDesktop)

#FILE(GLOB GIFFILES *.gif )
FILE(GLOB PNGFILES *.png )

#INSTALL(FILES ${GIFFILES}  DESTINATION  ${CMAKE_INSTALL_PREFIX}/theme/skins/default/widget/default/googleweather/)
INSTALL(FILES ${PNGFILES}  DESTINATION  ${CMAKE_INSTALL_PREFIX}/theme/skins/default/widget/default/googleweather/)
