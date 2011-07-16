#ADD_SUBDIRECTORY(artwork)
#ADD_SUBDIRECTORY(buttons)
#ADD_SUBDIRECTORY(icons/crystal)
ADD_SUBDIRECTORY(generic-widget)

#FILE(GLOB PNGFILES *.png)

INSTALL(FILES "artwork/icons/plexydesk.png" DESTINATION share/plexy)
#INSTALL(FILES ${PNGFILES} DESTINATION share/plexy/skins/default)
