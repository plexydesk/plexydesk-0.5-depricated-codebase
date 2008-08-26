PROJECT(PlexyDesktop)

FILE(GLOB PNGFILES *.png)

INSTALL(FILES ${PNGFILES}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/skins/default/)

add_subdirectory(buttons)
add_subdirectory(widget-example)

