PROJECT(PlexyDesktop)

SET(ICONS 128x128/apps
          128x128/devices
          128x128/filesystems
          128x128/mimetypes
          64x64/apps
          64x64/devices
          64x64/filesystems
          64x64/mimetypes
          48x48/apps
          48x48/devices
          48x48/filesystems
          48x48/mimetypes
          32x32/actions
          32x32/apps
          32x32/devices
          32x32/filesystems
          32x32/mimetypes
          16x16/actions
          16x16/apps
          16x16/devices
          16x16/filesystems
          16x16/mimetypes
          22x22/actions
          22x22/apps
          22x22/devices
          22x22/filesystems
          22x22/mimetypes
          )

foreach(X ${ICONS})
message("Process Icons.................${X}")
FILE(GLOB PNGFILES ${X}/*.*)
INSTALL(FILES ${PNGFILES}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/skins/default/icons/${X})
endforeach(X)

FILE(GLOB FILES_theme *.*)

INSTALL(FILES ${FILES_theme}  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/plexy/skins/default/icons/)
