PROJECT(PlexyDesktop)

FILE(GLOB PNGFILES *.png kbfxlayoutrc kbfxfontrc version-2 )

INSTALL(FILES ${PNGFILES}  DESTINATION /usr/share/plexy/skins/default/)

add_subdirectory(buttons)
add_subdirectory(widget-example)

