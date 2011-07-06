FILE(GLOB PNGFILES mimetypes version-2 )

INSTALL(FILES ${PNGFILES} DESTINATION share/plexy/mime)
