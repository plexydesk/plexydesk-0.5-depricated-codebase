VAR /GLOBAL APPNAME

Name "$APPNAME"

# define name of installer
OutFile "PlexyDeskop-0.1.1-win32.exe"
 
# define installation directory
InstallDir c:\plexydesk\

SetCompressor /SOLID lzma

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------
; Pages

Page components
;Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

; Optional section (can be disabled by the user)
Section "PlexyDesktop Shortcuts"

  CreateDirectory "$SMPROGRAMS\$APPNAME"
  CreateShortCut "$SMPROGRAMS\$APPNAME\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\$APPNAME\$APPNAME.lnk" "$INSTDIR\bin\$APPNAME.exe" "" "$INSTDIR\bin\plexydesk.exe" 0
  
SectionEnd

Section "Uninstall"
  
  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\$APPNAME\Uninstall.lnk"
  Delete "$SMPROGRAMS\$APPNAME\$APPNAME.lnk"
  RMDIR "$SMPROGRAMS\$APPNAME"

  RMDir /R "$INSTDIR"

SectionEnd

# start default section
section
   
  SetOutPath $INSTDIR
  SetOverwrite on
  FILE /r c:\installer\*.*
   
  # create the uninstaller
  writeUninstaller "$INSTDIR\uninstall.exe"
sectionEnd

Function .onInit

StrCpy $APPNAME "PlexyDeskop"

FunctionEnd