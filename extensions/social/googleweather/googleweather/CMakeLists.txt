FILE(GLOB PNGFILES *.png )
#FILE(GLOB GIFFILES *.gif )

INSTALL(FILES ${PNGFILES}  DESTINATION  ${CMAKE_INSTALL_PREFIX}/theme/skins/default/widget/default/googleweather)
#INSTALL(FILES ${GIFFILES}  DESTINATION  ${CMAKE_INSTALL_PREFIX}/theme/skins/default/widget/default/googleweather)
