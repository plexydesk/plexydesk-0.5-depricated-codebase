FILE(GLOB PNGFILES *.svg *.png )

INSTALL(FILES ${PNGFILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/skins/default/welcome)
