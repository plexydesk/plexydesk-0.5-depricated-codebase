; NSIS PlexyDesk installer script for Windows platform
; Author: PhobosK <phobosk@kbfx.net>
; License: LGPLv3
; Tested with Unicode NSIS 2.46



;--------------------------------
; Definitions
!define PRODUCT_NAME "@APPLICATION_NAME@"
!define PRODUCT_BIN_NAME "@APPLICATION_EXE_NAME@"
!define PRODUCT_VERSION "@APPLICATION_MAIN_VERSION@"
!define PRODUCT_PUBLISHER "@APPLICATION_NAME@ Team"
!define PRODUCT_WEB_SITE "http://www.plexyplanet.org/"
!define PRODUCT_WEB_SITE_SHORT "plexyplanet.org"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\${PRODUCT_BIN_NAME}.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_REGKEY "Software\${PRODUCT_WEB_SITE_SHORT}\${PRODUCT_BIN_NAME}"

; Choose a targeted windows version. We do not support versions less than 5.0 !
!define MINIMAL_OS "5.0"
;4.1 - target Windows 9x or more recent
;4.2 - target Windows NT4 or more recent
;5.0 - target Windows 2000 or more recent  / makes a Unicode installer
;5.1 - target Windows XP or more recent    / makes a Unicode installer
;6.0 - target Windows Vista or more recent / makes a Unicode installer
;6.1 - target Windows Seven or more recent / makes a Unicode installer

; URLs to Microsoft downloads for VC redistributables
!define VC10_REDIST_x86_URL \
    "http://download.microsoft.com/download/C/6/D/C6D0FD4E-9E53-4897-9B91-836EBA2AACD3/vcredist_x86.exe"
!define VC10_REDIST_x86_MD5 "CEDE02D7AF62449A2C38C49ABECC0CD3"
!define VC10_REDIST_x86_SEARCH "Microsoft Visual C++ 2010 Redistributable Package (x86)"

!define VC10_REDIST_x64_URL \
    "http://download.microsoft.com/download/A/8/0/A80747C3-41BD-45DF-B505-E9710D2744E0/vcredist_x64.exe"
!define VC10_REDIST_x64_MD5 "CBE0B05C11D5D523C2AF997D737C137B"
!define VC10_REDIST_x64_SEARCH "Microsoft Visual C++ 2010 Redistributable Package (x64)"

!define VC9_REDIST_x86_URL \
    "http://download.microsoft.com/download/d/d/9/dd9a82d0-52ef-40db-8dab-795376989c03/vcredist_x86.exe"
!define VC9_REDIST_x86_MD5 "5689D43C3B201DD3810FA3BBA4A6476A"
!define VC9_REDIST_x86_SEARCH "Microsoft Visual C++ 2008 Redistributable Package (x86)"

!define VC9_REDIST_x64_URL \
    "http://download.microsoft.com/download/2/d/6/2d61c766-107b-409d-8fba-c39e61ca08e8/vcredist_x64.exe"
!define VC9_REDIST_x64_MD5 "40395C175553CB14D2050888EFCCDF00"
!define VC9_REDIST_x64_SEARCH "Microsoft Visual C++ 2008 Redistributable Package (x64)"

; Command line invocation keys override
!ifndef PRODUCT_PLATFORM
  !define PRODUCT_PLATFORM "x86"
!endif

!ifndef PRODUCT_VC_VERSION
  !define PRODUCT_VC_VERSION "VC10"
!endif

!ifndef PRODUCT_DEBUG
  !define PRODUCT_DEBUG ""
!endif

!ifndef PRODUCT_SOURCES_PATH
  !define PRODUCT_SOURCES_PATH "@CMAKE_SOURCE_DIR@" ; Normalized later
!endif

!ifndef PRODUCT_BIN_SOURCES_PATH
  !define PRODUCT_BIN_SOURCES_PATH "@CMAKE_INSTALL_PREFIX@" ; Normalized later
!endif

; Check and fix command line parameters
!if ${PRODUCT_PLATFORM} == "x64"
  !if ${PRODUCT_VC_VERSION} == "VC9"
    !define VC_URL "${VC9_REDIST_x64_URL}"
    !define VC_MD5 "${VC9_REDIST_x64_MD5}"
    !define VC_SEARCH "${VC9_REDIST_x64_SEARCH}"
  !else
    !undef PRODUCT_VC_VERSION
    !define PRODUCT_VC_VERSION "VC10"
    !define VC_URL "${VC10_REDIST_x64_URL}"
    !define VC_MD5 "${VC10_REDIST_x64_MD5}"
    !define VC_SEARCH "${VC10_REDIST_x64_SEARCH}"
  !endif
!else
  !undef PRODUCT_PLATFORM
  !define PRODUCT_PLATFORM "x86"
  !if ${PRODUCT_VC_VERSION} == "VC9"
    !define VC_URL "${VC9_REDIST_x86_URL}"
    !define VC_MD5 "${VC9_REDIST_x86_MD5}"
    !define VC_SEARCH "${VC9_REDIST_x86_SEARCH}"
  !else
    !undef PRODUCT_VC_VERSION
    !define PRODUCT_VC_VERSION "VC10"
    !define VC_URL "${VC10_REDIST_x86_URL}"
    !define VC_MD5 "${VC10_REDIST_x86_MD5}"
    !define VC_SEARCH "${VC10_REDIST_x86_SEARCH}"
  !endif
!endif

var /GLOBAL REG_ROOT
var /GLOBAL KEEP_SETTINGS_UNINSTALL

; Normalize input paths, so we do not get errors
!searchreplace PRODUCT_SOURCES_PATH "${PRODUCT_SOURCES_PATH}" "/" "\"
!searchreplace PRODUCT_BIN_SOURCES_PATH "${PRODUCT_BIN_SOURCES_PATH}" "/" "\"

;--------------------------------
; Compressor
SetCompressor /SOLID lzma
SetCompressorDictSize 32


;--------------------------------
; Additional plugin folders. Uses CMake sources path for windows
!addplugindir "${PRODUCT_SOURCES_PATH}\dist\windows\nsis-unicode\plugins"
!addincludedir "${PRODUCT_SOURCES_PATH}\dist\windows\nsis-unicode\include"


;--------------------------------
; Add and configure multiuser install support.
!define MULTIUSER_EXECUTIONLEVEL Highest
!define MULTIUSER_MUI
!define MULTIUSER_INSTALLMODE_COMMANDLINE
!define MULTIUSER_PLATFORM ${PRODUCT_PLATFORM}
!define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_KEY "${PRODUCT_REGKEY}"
!define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_VALUENAME "NSIS:MultiUser"
!define MULTIUSER_INSTALLMODE_INSTDIR "${PRODUCT_NAME}"
!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY "${PRODUCT_REGKEY}"
!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_VALUENAME "NSIS:InstallPath_${PRODUCT_PLATFORM}"
!include "${PRODUCT_SOURCES_PATH}\dist\windows\nsis-unicode\include\MultiUser.nsh"
!include "MUI2.nsh"

;--------------------------------
; Include other functions etc. support
!include "WordFunc.nsh"
!include "EnvVarUpdate.nsh"
!include "FileFunc.nsh"
!insertmacro GetParameters
!insertmacro GetOptions

;--------------------------------
; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ABORTWARNING_CANCEL_DEFAULT
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Choose install mode page
!insertmacro MULTIUSER_PAGE_INSTALLMODE

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT SHELL_CONTEXT
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_REGKEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"
!define MUI_LANGDLL_ALLLANGUAGES
!define MUI_LANGDLL_ALWAYSSHOW

; Welcome page
!insertmacro MUI_PAGE_WELCOME

; License page
!insertmacro MUI_PAGE_LICENSE "${PRODUCT_SOURCES_PATH}\COPYING"

; Components page
!insertmacro MUI_PAGE_COMPONENTS

; Directory page
!insertmacro MUI_PAGE_DIRECTORY

; Start menu page
var /GLOBAL ICONS_GROUP
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "${PRODUCT_NAME}"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT SHELL_CONTEXT
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_REGKEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "NSIS:StartMenuDir"
!insertmacro MUI_PAGE_STARTMENU Application "$ICONS_GROUP"

; Instfiles page
!insertmacro MUI_PAGE_INSTFILES

; Finish page
!define MUI_FINISHPAGE_NOAUTOCLOSE
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!define MUI_UNFINISHPAGE_NOAUTOCLOSE
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; Reserve files (go to header block if SOLID key is used in compression)
!insertmacro MUI_RESERVEFILE_LANGDLL
ReserveFile '${NSISDIR}\Plugins\InstallOptions.dll'

; Language files
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Bulgarian"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Japanese"
!insertmacro MUI_LANGUAGE "Portuguese"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "Spanish"

; MUI end
;--------------------------------


;--------------------------------
; Main settings
Name "${PRODUCT_NAME}(${PRODUCT_PLATFORM}) ${PRODUCT_VERSION}"
OutFile "${PRODUCT_SOURCES_PATH}\INSTALLERS\${PRODUCT_NAME}_${PRODUCT_VERSION}-Installer_${PRODUCT_PLATFORM}_${PRODUCT_VC_VERSION}${PRODUCT_DEBUG}.exe"

; Installer file version tab properties
VIProductVersion "${PRODUCT_VERSION}.0"
VIAddVersionKey "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey "ProductVersion" "${PRODUCT_VERSION}"
VIAddVersionKey "FileVersion" "${PRODUCT_VERSION}"
VIAddVersionKey "CompanyName" "${PRODUCT_PUBLISHER}"
VIAddVersionKey "LegalCopyright" "LGPL v3"
VIAddVersionKey "FileDescription" "${PRODUCT_NAME} Windows Installer"
VIAddVersionKey "Comments" "A Qt4 customised desktop"

BrandingText "${PRODUCT_NAME}(${PRODUCT_PLATFORM}) for Windows v${PRODUCT_VERSION}${PRODUCT_DEBUG}"

; Unicode will be implemented in NSIS 2.50. Comment till then to avoid errors.
;TargetMinimalOS ${MINIMAL_OS}
InstType "Normal"
InstType "Full"
ShowInstDetails show
ShowUnInstDetails show


;--------------------------------
; Functions
Function .onInit
  ; Make sure only one installer is running
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "${PRODUCT_NAME}InstMutex") i .r1 ?e'
  Pop $R0

  StrCmp $R0 0 +3
  MessageBox MB_OK|MB_ICONEXCLAMATION \
             "The ${PRODUCT_NAME} Installer is already running."
  Abort

  ClearErrors

  !insertmacro MULTIUSER_INIT

  ; Check in which install mode we run and set the necessary Registry Root Key
  ; that all actions will be upon. This is needed for modifying PATH env.
  ${If} $MultiUser.InstallMode == "AllUsers"
    StrCpy $REG_ROOT "HKLM"
  ${Else}
    StrCpy $REG_ROOT "HKCU"
  ${EndIf}

  !insertmacro MUI_LANGDLL_DISPLAY

  ; Check to see if there is an old installed version of PlexyDesk
  ; First check with the same install mode (HKLM or HKCU depending on the mode)
  ReadRegStr $R0 SHELL_CONTEXT \
             "${PRODUCT_UNINST_KEY}" \
             "UninstallString"

  ${If} $R0 != ""
      MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
                 "${PRODUCT_NAME} is already installed. \
                 $\nYou should remove the already installed version first. \
                 $\n$\nClick `OK` to remove the existing version or `Cancel` to cancel this installation." \
                 /SD IDOK \
                 IDOK +2
      Abort

      ${WordFind} "$R0" "\" "-2{*" $R3
      ${WordReplace} $R3 '"' "" "+" $R3

      ClearErrors
      ExecWait '"$R0" /KEEP_SETTINGS _?=$R3' ;Run the uninstaller of the old version

      IfErrors +1 remove_uninstaller
      MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
                 "The old installation of ${PRODUCT_NAME} could't be removed. \
                 $\n$\nClick `OK` to continue with this installation or `Cancel` to cancel the installation." \
                 /SD IDOK \
                 IDOK done_install_mode
      Abort

      remove_uninstaller:
      Delete "$R3\*.*"
      RMDir "$R3"

      done_install_mode:

  ${EndIf}


  ; Next check if it is installed in the oposite mode.
  ; Do this only if we have permissions
  ${if} $MultiUser.Privileges == "Admin"
  ${orif} $MultiUser.Privileges == "Power"
    ${If} $REG_ROOT == "HKLM"
      ReadRegStr $R0 "HKCU" \
                 "${PRODUCT_UNINST_KEY}" \
                 "UninstallString"
    ${Else}
      ReadRegStr $R0 "HKLM" \
                 "${PRODUCT_UNINST_KEY}" \
                 "UninstallString"
    ${EndIf}

    ${If} $R0 != ""
        MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
                   "${PRODUCT_NAME} is already installed for another user. \
                   $\nYou should remove the already installed version first. \
                   $\n$\nClick `OK` to remove the existing version or `Cancel` to cancel this installation." \
                   /SD IDOK \
                   IDOK +2
        Abort

        ${WordFind} "$R0" "\" "-2{*" $R3
        ${WordReplace} $R3 '"' "" "+" $R3

        ClearErrors
        ExecWait '"$R0" /KEEP_SETTINGS _?=$R3' ;Run the uninstaller of the old version


        IfErrors +1 remove_installer_2
        MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
                   "The old installation of ${PRODUCT_NAME}for your other user could't be removed. \
                   $\n$\nClick `OK` to continue with this installation or `Cancel` to cancel the installation." \
                   /SD IDOK \
                   IDOK done_all
        Abort

        remove_installer_2:
        Delete "$R3\*.*"
        RMDir "$R3"

        done_all:

    ${EndIf}
  ${endif}

FunctionEnd


Function un.onInit
  ; Make sure only one uninstaller is running
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "${PRODUCT_NAME}UninstMutex") i .r1 ?e'
  Pop $R0

  StrCmp $R0 0 +3
  MessageBox MB_OK|MB_ICONEXCLAMATION \
             "The ${PRODUCT_NAME} Uninstaller is already running."
  Abort

  ClearErrors

  ; Parse command line parameter /KEEP_SETTINGS
  StrCpy $KEEP_SETTINGS_UNINSTALL "FALSE"
  ${GetParameters} $R0
  ClearErrors
  ${GetOptions} $R0 /KEEP_SETTINGS $0
  IfErrors +2
    StrCpy $KEEP_SETTINGS_UNINSTALL "TRUE"

  ClearErrors

  !insertmacro MULTIUSER_UNINIT

  ; Check in which uninstall mode we run and set the necessary Registry Root Key
  ; that all actions will be upon. This is needed for modifying PATH env.
  ${If} $MultiUser.InstallMode == "AllUsers"
    StrCpy $REG_ROOT "HKLM"
  ${Else}
    StrCpy $REG_ROOT "HKCU"
  ${EndIf}

  ; With the current install mode (got from registry's HKLM or HKCU), check to
  ; see if there is an uninstall information for PlexyDesk. If no such info
  ; go into the oposite mode
  ReadRegStr $R0 SHELL_CONTEXT \
             "${PRODUCT_UNINST_KEY}" \
             "UninstallString"

  ${If} $R0 == ""
    ${If} $REG_ROOT == "HKLM"
      Call un.MultiUser.InstallMode.CurrentUser
      StrCpy $REG_ROOT "HKCU"
    ${Else}
      Call un.MultiUser.InstallMode.AllUsers
      StrCpy $REG_ROOT "HKLM"
    ${EndIf}
  ${EndIf}

  ; Check if oposite mode has an uninstall info. If not bring error and abort.
  ReadRegStr $R0 SHELL_CONTEXT \
             "${PRODUCT_UNINST_KEY}" \
             "UninstallString"

  ${If} $R0 == ""
      MessageBox MB_OK|MB_ICONEXCLAMATION|MB_SETFOREGROUND \
                 "Any uninstall information for ${PRODUCT_NAME} is missing. \
                 $\n$\nThe application cannot be uninstalled. \
                 $\n$\nPlease click `OK` to exit the uninstallation."
      Abort
  ${EndIf}


  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd


;--------------------------------
; Sections descriptions
;
; INSTALL SECTIONS
Section "!${PRODUCT_NAME} Application" SEC01
  ; Selected for "Full" and "Normal" installs. Always used.
  SectionIn 1 2 RO

  ; Copy files to install path
  DetailPrint "Copying ${PRODUCT_NAME}'s files to the install destination..."
  SetOutPath "$INSTDIR"
  ${If} ${PRODUCT_DEBUG} == ""
    File /r /x *.lib /x *.obj /x *.res /x *.exp /x *.ilk /x *.pdb "${PRODUCT_BIN_SOURCES_PATH}\*.*"
  ${Else}
    File /r /x *.lib /x *.obj /x *.res /x *.exp "${PRODUCT_BIN_SOURCES_PATH}\*.*"
  ${EndIf}
SectionEnd


Section "Microsoft VC files (Webinstall)" SEC02
  ; Selected for "Full" install only. Used in "Full" install.
  SectionIn 2

  ; Download Microsoft VC if selected
  retry_download:
  DetailPrint "Downloading Microsoft ${PRODUCT_VC_VERSION} (${PRODUCT_PLATFORM})..."
  InitPluginsDir
  NSISdl::download /TIMEOUT=30000 "${VC_URL}" \
             "$PLUGINSDIR\${PRODUCT_VC_VERSION}_redist_${PRODUCT_PLATFORM}.exe"
  Pop $R0

  StrCmp $R0 "success" check_file

  ; Getting not OK does not necessarily mean there was a download error, so check first.
  ${If} $R0 == "cancel"
    DetailPrint "The download CANCELED!"
    MessageBox MB_YESNO|MB_ICONQUESTION|MB_SETFOREGROUND \
               "Cancel the download of Microsoft ${PRODUCT_VC_VERSION} (${PRODUCT_PLATFORM})?" \
               /SD IDYES \
               IDNO retry_download \
               IDYES install_done
  ${Else}
    DetailPrint "The download FAILED!"
    MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION|MB_SETFOREGROUND \
               "The download failed with this error:$\n $R0 $\n \
               $\nShall we retry the download? \
               $\n(If this is not the first time you get this message, please choose the CANCEL button.)" \
               /SD IDCANCEL \
               IDRETRY retry_download \
               IDCANCEL section_failed
  ${EndIf}

  check_file:
  ; Generate MD5sum of the file and compare it to original MD5 sum
  md5dll::GetMD5File "$PLUGINSDIR\${PRODUCT_VC_VERSION}_redist_${PRODUCT_PLATFORM}.exe"
  Pop $0
  StrCmp $0 "${VC_MD5}" download_done

  MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION|MB_SETFOREGROUND \
             "The downloaded Microsoft ${PRODUCT_VC_VERSION} (${PRODUCT_PLATFORM}) is broken! \
             $\nShall we retry the download? \
             $\n(If this is not the first time you get this message, please choose the CANCEL button.)" \
             /SD IDCANCEL \
             IDRETRY retry_download \
             IDCANCEL section_failed

  download_done:

  ; Install the Microsoft VC
  retry_install:
  DetailPrint "Installing Microsoft ${PRODUCT_VC_VERSION} (${PRODUCT_PLATFORM})..."
  ClearErrors
  ExecWait '"$PLUGINSDIR\${PRODUCT_VC_VERSION}_redist_${PRODUCT_PLATFORM}.exe"'
  IfErrors 0 install_done
    MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION|MB_SETFOREGROUND \
               "The installation of Microsoft ${PRODUCT_VC_VERSION} (${PRODUCT_PLATFORM}) FAILED! \
               $\nShall we retry the installation? \
               $\n(If this is not the first time you get this message, please choose the CANCEL button.)" \
               /SD IDCANCEL \
               IDRETRY retry_install

  section_failed:
    MessageBox MB_OK|MB_ICONEXCLAMATION|MB_SETFOREGROUND \
               "The use of the Microsoft ${PRODUCT_VC_VERSION} (${PRODUCT_PLATFORM}) component FAILED! \
               $\nWhen this Installer finishes, please try to manually download the Microsoft \
               ${PRODUCT_VC_VERSION} (${PRODUCT_PLATFORM}) component from the Microsoft Homepage. \
               $\nThere you can either search for $\"${VC_SEARCH}$\", or use this direct download link: \
               $\n${VC_URL}"

  install_done:
SectionEnd

; Section Descriptions showed to the user
LangString DESC_SEC01 ${LANG_ENGLISH} "Install all the needed ${PRODUCT_NAME} files."
LangString DESC_SEC02 ${LANG_ENGLISH} "Download and Install the VC redistribution package from Microsoft Homepage."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} $(DESC_SEC01)
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} $(DESC_SEC02)
!insertmacro MUI_FUNCTION_DESCRIPTION_END


Section -Post
  ; Refresh in which install mode we run and set the necessary Registry Root Key
  ; that all actions will be upon. This is needed for modifying PATH env.
  ; Refresh is needed 'cause the user could've changed it in install mode page.
  ${If} $MultiUser.InstallMode == "AllUsers"
    StrCpy $REG_ROOT "HKLM"
  ${Else}
    StrCpy $REG_ROOT "HKCU"
  ${EndIf}

  ; Selected for both "Full" and "Normal" install. Always used. Invisible.
  SectionIn 1 2 RO

  ; Add the PlexyDesk Uninstaller
  DetailPrint "Adding the ${PRODUCT_NAME} Uninstaller..."
  WriteUninstaller "$INSTDIR\uninst.exe"

  ; Create the PlexyDesk shortcuts
  DetailPrint "Creating the necessary ${PRODUCT_NAME}'s shortcuts..."
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  StrCpy $R0 $OUTDIR ; Just remember our working dir
  SetOutPath "$INSTDIR\bin" ; A trick to set the working folder of the shortcuts
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\${PRODUCT_NAME}.lnk" "$INSTDIR\bin\${PRODUCT_BIN_NAME}.exe" \
                 "" "$INSTDIR\${PRODUCT_BIN_NAME}.ico" "" "" "" "Start ${PRODUCT_NAME}"
  CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\bin\${PRODUCT_BIN_NAME}.exe" \
                 "" "$INSTDIR\${PRODUCT_BIN_NAME}.ico" "" "" "" "Start ${PRODUCT_NAME}"
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\${PRODUCT_NAME} Online.lnk" "$INSTDIR\${PRODUCT_NAME}.url" \
                 "" "$INSTDIR\${PRODUCT_BIN_NAME}.ico" "" "" "" "Visit ${PRODUCT_NAME}'s Homepage"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall ${PRODUCT_NAME}.lnk" "$INSTDIR\uninst.exe" \
                 "" "" "" "" "" "Uninstall ${PRODUCT_NAME}"
  SetOutPath "$R0"
  !insertmacro MUI_STARTMENU_WRITE_END

  ; Add the necessary PlexyDesk Registry Keys
  DetailPrint "Adding the necessary ${PRODUCT_NAME} Registry Keys..."
  WriteRegStr SHELL_CONTEXT "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\bin\${PRODUCT_BIN_NAME}.exe"
  WriteRegStr SHELL_CONTEXT "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr SHELL_CONTEXT "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr SHELL_CONTEXT "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\${PRODUCT_BIN_NAME}.ico"
  WriteRegStr SHELL_CONTEXT "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr SHELL_CONTEXT "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr SHELL_CONTEXT "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  WriteRegDWORD SHELL_CONTEXT "${PRODUCT_UNINST_KEY}" "NoModify" 1
  WriteRegDWORD SHELL_CONTEXT "${PRODUCT_UNINST_KEY}" "NoRepair" 1

  ; Add multiuser registry keys
  WriteRegStr SHELL_CONTEXT "${PRODUCT_REGKEY}" "NSIS:MultiUser" "$MultiUser.InstallMode"
  WriteRegStr SHELL_CONTEXT "${PRODUCT_REGKEY}" "NSIS:InstallPath_${PRODUCT_PLATFORM}" "$INSTDIR"

  ; Modify qt.conf. QT uses it properly only if it has Unix path delimiters
  DetailPrint "Modifying QT configuration file to work with ${PRODUCT_NAME} ..."
  ${WordReplace} " $INSTDIR\lib\qt4" "\" "/" "+*" $R5
  WriteINIStr "$INSTDIR\bin\qt.conf" "Paths" "Prefix" $R5

  ; Add lib to PATH env variable
  DetailPrint "Adding ${PRODUCT_NAME} to PATH..."
  ${EnvVarUpdate} $0 "PATH" "A" "$REG_ROOT" "$INSTDIR\lib"
SectionEnd


; UNINSTALL SECTIONS
Section Uninstall
  ; Close any open instances of PlexyDesk
  !include WinMessages.nsh
  DetailPrint "Closing all ${PRODUCT_NAME} running instances..."
  FindWindow $0 "QWidget" "${PRODUCT_NAME}"
  SendMessage $0 ${WM_CLOSE} 0 0

  ; Remove all PlexyDesk shortcuts
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" "$ICONS_GROUP"

  DetailPrint "Removing all ${PRODUCT_NAME} shortcuts..."
  Delete "$SMPROGRAMS\$ICONS_GROUP\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\${PRODUCT_NAME} Online.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Uninstall ${PRODUCT_NAME}.lnk"
  Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
  RMDir "$SMPROGRAMS\$ICONS_GROUP"

  ; Remove all PlexyDesk installed files
  DetailPrint "Removing all files installed by ${PRODUCT_NAME}..."
  Delete "$INSTDIR\*.*"
  RMDir /r "$INSTDIR\bin"
  RMDir /r "$INSTDIR\lib"
  RMDir /r "$INSTDIR\share"
  RMDir "$INSTDIR"

  ; Remove all PlexyDesk Registry entries
  DetailPrint "Removing all ${PRODUCT_NAME} Registry Keys..."
  DeleteRegKey SHELL_CONTEXT "${PRODUCT_UNINST_KEY}"
  DeleteRegKey SHELL_CONTEXT "${PRODUCT_DIR_REGKEY}"

  ; Keep installer settings? This will not appear if it is an uninstall
  ; suggestion during install.
  StrCmp $KEEP_SETTINGS_UNINSTALL "TRUE" keep_settings
  MessageBox MB_YESNO|MB_ICONQUESTION \
             "Would you like to keep your ${PRODUCT_NAME} settings?" \
             /SD IDNO \
             IDYES keep_settings
  DeleteRegKey SHELL_CONTEXT "${PRODUCT_REGKEY}"

  keep_settings:
  ; Remove lib from PATH env variable
  DetailPrint "Removing ${PRODUCT_NAME} from PATH..."
  ${un.EnvVarUpdate} $0 "PATH" "R" "$REG_ROOT" "$INSTDIR\lib"
SectionEnd
