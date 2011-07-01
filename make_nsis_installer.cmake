VAR /GLOBAL APPNAME
VAR /GLOBAL APPBIN

Name "$APPNAME"

# define name of installer
OutFile "${APPLICATION_NAME}-${APPLICATION_MAIN_VERSION}.exe"
 
# define installation directory
InstallDir "c:\plexydesk\"

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
  CreateShortCut "$SMPROGRAMS\$APPNAME\$APPNAME.lnk" "$INSTDIR\bin\$APPBIN.exe" "" "$INSTDIR\bin\$APPBIN.exe" 0
  
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
  FILE /r c:\plexydesk\*.*
   
  # create the uninstaller
  writeUninstaller "c:\$APPBIN\uninstall.exe"
sectionEnd

Function .onInit

StrCpy $APPNAME "${APPLICATION_NAME}"
StrCpy $APPBIN "plexydesk"

FunctionEnd
