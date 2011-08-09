@echo off

rem Script for building and packing PlexyDesk on Windows Platform
rem Authors: 
rem 	PhobosK <phobosk@kbfx.net>
rem		Dariusz Mikulski <dariusz.mikulski@gmail.com>
rem License: LGPLv3


rem QT x86 configured with MS VS 2010 with the folowing options:
rem configure -release -opensource -qt-zlib -qt-libpng -qt-libmng -qt-libtiff -qt-libjpeg -platform win32-msvc2010

rem QT x64 configured with MS VS 2010 with the folowing options:
rem configure -release -opensource -qt-zlib -qt-libpng -qt-libmng -qt-libtiff -qt-libjpeg


CLS
set CURRENT_PATH=%cd%

rem Make sure we do all operations from the GIT source path
cd /d %~dp0
cd ..
set GIT_PATH=%cd%

rem First we create a folder where all Installers will go
IF EXIST "%GIT_PATH%\INSTALLERS" GOTO create_build
echo Creating an INSTALLERS folder ...
echo.
mkdir INSTALLERS

:create_build
rem We better build out of the sources
IF EXIST "%GIT_PATH%\build" GOTO go_to_build
echo Creating a build folder ...
echo.
mkdir build

:go_to_build
echo Make sure you have installed NSIS from http://nsis.sourceforge.net
echo and added path to makensis.exe to your PATH environment
echo.
echo.
echo.
echo Entering the build folder ...
echo.
cd build

rem The directory where all will be installed before packed
set FINAL_PATH_NAME=plexydesk-ready-build
set FINAL_PATH=%cd%\%FINAL_PATH_NAME%
set FINAL_PATH_FIXED=%FINAL_PATH:\=/%

IF EXIST "%FINAL_PATH%" GOTO go_on
mkdir "%FINAL_PATH%"

:go_on
echo.
echo 1. MS Visual Studio 10 2010
echo 2. MS Visual Studio 10 2010 (x64)
echo 3. MS Visual Studio 9 2008
echo 4. MS Visual Studio 9 2008 (x64)
echo 5. MinGW
echo.

CHOICE /M "Please choose which version of MS VC to use [default is 1]: " /C 12345 /CS /D 1 /T 20 /N
if errorlevel 5 goto mingw
if errorlevel 4 goto vc9x64
if errorlevel 3 goto vc9
if errorlevel 2 goto vc10x64
if errorlevel 1 goto vc10
echo.

:vc9x64
echo Setting version to: MS Visual Studio 9 2008 (x64)
echo.
set MSVC_VERSION=Visual Studio 9 2008 Win64
set PROJECT_EXT=vcproj
set PROJECT_PLATFORM=x64
set PROJECT_VC=VC9
goto end_compiler_type

:vc9
echo Setting version to: MS Visual Studio 9 2008
echo.
set MSVC_VERSION=Visual Studio 9 2008
set PROJECT_EXT=vcproj
set PROJECT_PLATFORM=x86
set PROJECT_VC=VC9
goto end_compiler_type

:vc10x64
echo Setting version to: MS Visual Studio 10 2010 (x64)
echo.
set MSVC_VERSION=Visual Studio 10 Win64
set PROJECT_EXT=vcxproj
set PROJECT_PLATFORM=x64
set PROJECT_VC=VC10
goto end_compiler_type

:vc10
echo Setting version to: MS Visual Studio 10 2010
echo.
set MSVC_VERSION=Visual Studio 10
set PROJECT_EXT=vcxproj
set PROJECT_PLATFORM=x86
set PROJECT_VC=VC10
goto end_compiler_type

:mingw
echo Setting version to: MinGW Makefiles
echo.
set MINGW_VERSION=MinGW Makefiles
goto end_compiler_type_mingw

:end_compiler_type
echo.
echo 1. Release
echo 2. RelWithDebInfo
echo 3. MinSizeRel
echo 4. Debug
echo.
goto release_type

:end_compiler_type_mingw
echo.
echo 1. Release
echo 2. Debug
echo.
goto release_type_mingw

:release_type
CHOICE /M "Please choose the release type to use [default is 1]: " /C 1234 /CS /D 1 /T 20 /N
if errorlevel 4 goto dbg
if errorlevel 3 goto msr
if errorlevel 2 goto rdbg
if errorlevel 1 goto rel
echo.

:dbg
echo Setting release type to: Debug
echo.
set RELEASE_TYPE=Debug
set PROJECT_DEBUG=debug
goto end_rel_type

:msr
echo Setting release type to: MinSizeRel
echo.
set RELEASE_TYPE=MinSizeRel
set PROJECT_DEBUG=none
goto end_rel_type

:rdbg
echo Setting release type to: RelWithDebInfo
echo.
set RELEASE_TYPE=RelWithDebInfo
set PROJECT_DEBUG=debug
goto end_rel_type

:rel
echo Setting release type to: Release
echo.
set RELEASE_TYPE=Release
set PROJECT_DEBUG=none
goto end_rel_type

:release_type_mingw
CHOICE /M "Please choose the release type to use [default is 1]: " /C 12 /CS /D 1 /T 20 /N
if errorlevel 2 goto deb_mingw
if errorlevel 1 goto rel_mingw
echo.

:rel_mingw
set RELEASE_TYPE=Release
set PROJECT_DEBUG=none
goto end_compiler_debug_mingw

:deb_mingw
set RELEASE_TYPE=Debug
set PROJECT_DEBUG=debug
goto end_compiler_debug_mingw

:end_rel_type
echo.
echo 1. Quiet
echo 2. Minimal
echo 3. Normal
echo 4. Detailed
echo.

CHOICE /M "Please choose the debug level of the compilation [default is 1]: " /C 1234 /CS /D 1 /T 20 /N
if errorlevel 4 goto detailed
if errorlevel 3 goto normal
if errorlevel 2 goto minimal
if errorlevel 1 goto quiet
echo.

:detailed
echo Setting compilation debug level to: Detailed
echo.
set COMPILER_DEBUG_LEVEL=d
goto end_compiler_debug

:normal
echo Setting compilation debug level to: Normal
echo.
set COMPILER_DEBUG_LEVEL=n
goto end_compiler_debug

:minimal
echo Setting compilation debug level to: Minimal
echo.
set COMPILER_DEBUG_LEVEL=m
goto end_compiler_debug

:quiet
echo Setting compilation debug level to: Quiet
echo.
set COMPILER_DEBUG_LEVEL=q

:end_compiler_debug
echo.
echo Creating files with CMake ...
echo.
set STAGE=CREATING CMAKE FILES
cmake ../ -DCMAKE_INSTALL_PREFIX="%FINAL_PATH_FIXED%" -DCMAKE_BUILD_TYPE="%RELEASE_TYPE%" -G "%MSVC_VERSION%"
if %ERRORLEVEL% NEQ 0 goto ERROR


echo.
echo Building project ...
echo.
set STAGE=BUILDING THE PROJECT
msbuild PlexyDesktop.sln /v:%COMPILER_DEBUG_LEVEL% /t:Build /p:Configuration=%RELEASE_TYPE% /nologo
if %ERRORLEVEL% NEQ 0 goto ERROR


echo.
echo Installing project ...
echo.
set STAGE=INSTALLING THE PROJECT
msbuild /target:Build /v:%COMPILER_DEBUG_LEVEL% /p:Configuration=%RELEASE_TYPE% INSTALL.%PROJECT_EXT% /nologo
if %ERRORLEVEL% NEQ 0 goto ERROR

:finalize
echo.
echo Copying QT files to %FINAL_PATH% ...
echo.
set STAGE=COPYING QT FILES

if /i %PROJECT_DEBUG%==none goto copy_release_qt_libs
if /i %PROJECT_DEBUG%==debug goto copy_debug_qt_libs

rem this should be never happen !!!!
goto ERROR

:copy_release_qt_libs
xcopy /Y "%QTDIR%\lib\phonon4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\Qt3Support4.dll" "%FINAL_PATH%\lib\qt4\lib\"            
xcopy /Y "%QTDIR%\lib\QtCLucene4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtCore4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtDeclarative4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtDesigner4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtDesignerComponents4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtGui4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtHelp4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtMultimedia4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtNetwork4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtOpenGL4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtScript4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtScriptTools4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtSql4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtSvg4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtTest4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtWebKit4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtXml4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtXmlPatterns4.dll" "%FINAL_PATH%\lib\qt4\lib\"

xcopy /Y "%QTDIR%\imports\Qt\labs\folderlistmodel\qmldir" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\folderlistmodel\"
xcopy /Y "%QTDIR%\imports\Qt\labs\folderlistmodel\qmlfolderlistmodelplugin.dll" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\folderlistmodel\"

xcopy /Y "%QTDIR%\imports\Qt\labs\gestures\qmldir" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\gestures\"
xcopy /Y "%QTDIR%\imports\Qt\labs\gestures\qmlgesturesplugin.dll" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\gestures\"

xcopy /Y "%QTDIR%\imports\Qt\labs\particles\qmldir" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\particles\"
xcopy /Y "%QTDIR%\imports\Qt\labs\particles\qmlparticlesplugin.dll" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\particles\"

xcopy /Y "%QTDIR%\imports\QtWebKit\qmldir" "%FINAL_PATH%\lib\qt4\imports\QtWebKit\"
xcopy /Y "%QTDIR%\imports\QtWebKit\qmlwebkitplugin.dll" "%FINAL_PATH%\lib\qt4\imports\QtWebKit\"

xcopy /Y "%QTDIR%\plugins\accessible\qtaccessiblecompatwidgets4.dll" "%FINAL_PATH%\lib\qt4\plugins\accessible\"
xcopy /Y "%QTDIR%\plugins\accessible\qtaccessiblewidgets4.dll" "%FINAL_PATH%\lib\qt4\plugins\accessible\"

xcopy /Y "%QTDIR%\plugins\bearer\qtgenericbearer4.dll" "%FINAL_PATH%\lib\qt4\plugins\bearer\"
xcopy /Y "%QTDIR%\plugins\bearer\qtnativewifibearer4.dll" "%FINAL_PATH%\lib\qt4\plugins\bearer\"

xcopy /Y "%QTDIR%\plugins\codecs\qcncodecs4.dll" "%FINAL_PATH%\lib\qt4\plugins\codecs\"
xcopy /Y "%QTDIR%\plugins\codecs\qjpcodecs4.dll" "%FINAL_PATH%\lib\qt4\plugins\codecs\"
xcopy /Y "%QTDIR%\plugins\codecs\qkrcodecs4.dll" "%FINAL_PATH%\lib\qt4\plugins\codecs\"
xcopy /Y "%QTDIR%\plugins\codecs\qtwcodecs4.dll" "%FINAL_PATH%\lib\qt4\plugins\codecs\"

rem only release dll's
xcopy /Y "%QTDIR%\plugins\designer\phononwidgets.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"
xcopy /Y "%QTDIR%\plugins\designer\qaxwidget.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"
xcopy /Y "%QTDIR%\plugins\designer\qdeclarativeview.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"
xcopy /Y "%QTDIR%\plugins\designer\qt3supportwidget.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"
xcopy /Y "%QTDIR%\plugins\designer\qwebview.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"

xcopy /Y "%QTDIR%\plugins\graphicssystem\qglgraphicssystem4.dll" "%FINAL_PATH%\lib\qt4\plugins\graphicssystem\"
xcopy /Y "%QTDIR%\plugins\graphicssystem\qtracegraphicssystem4.dll" "%FINAL_PATH%\lib\qt4\plugins\graphicssystem\"

xcopy /Y "%QTDIR%\plugins\iconengines\qsvgicon4.dll" "%FINAL_PATH%\lib\qt4\plugins\iconengines\"

xcopy /Y "%QTDIR%\plugins\imageformats\qgif4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy /Y "%QTDIR%\plugins\imageformats\qico4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy /Y "%QTDIR%\plugins\imageformats\qjpeg4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy /Y "%QTDIR%\plugins\imageformats\qmng4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy /Y "%QTDIR%\plugins\imageformats\qsvg4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy /Y "%QTDIR%\plugins\imageformats\qtiff4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"

xcopy /Y "%QTDIR%\plugins\phonon_backend\phonon_ds94.dll" "%FINAL_PATH%\lib\qt4\plugins\phonon_backend\"

xcopy /Y "%QTDIR%\plugins\qmltooling\tcpserver4.dll" "%FINAL_PATH%\lib\qt4\plugins\qmltooling\"

rem Add here your additional Qt sql drivers
xcopy /Y "%QTDIR%\plugins\sqldrivers\qsqlite4.dll" "%FINAL_PATH%\lib\qt4\plugins\qsqldrivers\"

goto create_nsis

:copy_debug_qt_libs
xcopy /Y "%QTDIR%\lib\phonond4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\Qt3Supportd4.dll" "%FINAL_PATH%\lib\qt4\lib\"            
xcopy /Y "%QTDIR%\lib\QtCLucened4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtCored4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtDeclaratived4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtDesignerd4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtDesignerComponentsd4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtGuid4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtHelpd4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtMultimediad4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtNetworkd4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtOpenGLd4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtScriptd4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtScriptToolsd4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtSqld4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtSvgd4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtTestd4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtWebKitd4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtXmld4.dll" "%FINAL_PATH%\lib\qt4\lib\"
xcopy /Y "%QTDIR%\lib\QtXmlPatternsd4.dll" "%FINAL_PATH%\lib\qt4\lib\"

xcopy /Y "%QTDIR%\imports\Qt\labs\folderlistmodel\qmldir" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\folderlistmodel\"
xcopy /Y "%QTDIR%\imports\Qt\labs\folderlistmodel\qmlfolderlistmodelplugind.dll" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\folderlistmodel\"

xcopy /Y "%QTDIR%\imports\Qt\labs\gestures\qmldir" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\gestures\"
xcopy /Y "%QTDIR%\imports\Qt\labs\gestures\qmlgesturesplugind.dll" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\gestures\"

xcopy /Y "%QTDIR%\imports\Qt\labs\particles\qmldir" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\particles\"
xcopy /Y "%QTDIR%\imports\Qt\labs\particles\qmlparticlesplugind.dll" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\particles\"

xcopy /Y "%QTDIR%\imports\QtWebKit\qmldir" "%FINAL_PATH%\lib\qt4\imports\QtWebKit\"
xcopy /Y "%QTDIR%\imports\QtWebKit\qmlwebkitplugind.dll" "%FINAL_PATH%\lib\qt4\imports\QtWebKit\"

xcopy /Y "%QTDIR%\plugins\accessible\qtaccessiblecompatwidgetsd4.dll" "%FINAL_PATH%\lib\qt4\plugins\accessible\"
xcopy /Y "%QTDIR%\plugins\accessible\qtaccessiblewidgetsd4.dll" "%FINAL_PATH%\lib\qt4\plugins\accessible\"

xcopy /Y "%QTDIR%\plugins\bearer\qtgenericbearerd4.dll" "%FINAL_PATH%\lib\qt4\plugins\bearer\"
xcopy /Y "%QTDIR%\plugins\bearer\qtnativewifibearerd4.dll" "%FINAL_PATH%\lib\qt4\plugins\bearer\"

xcopy /Y "%QTDIR%\plugins\codecs\qcncodecsd4.dll" "%FINAL_PATH%\lib\qt4\plugins\codecs\"
xcopy /Y "%QTDIR%\plugins\codecs\qjpcodecsd4.dll" "%FINAL_PATH%\lib\qt4\plugins\codecs\"
xcopy /Y "%QTDIR%\plugins\codecs\qkrcodecsd4.dll" "%FINAL_PATH%\lib\qt4\plugins\codecs\"
xcopy /Y "%QTDIR%\plugins\codecs\qtwcodecsd4.dll" "%FINAL_PATH%\lib\qt4\plugins\codecs\"

rem only release dll's
xcopy /Y "%QTDIR%\plugins\designer\phononwidgets.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"
xcopy /Y "%QTDIR%\plugins\designer\qaxwidget.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"
xcopy /Y "%QTDIR%\plugins\designer\qdeclarativeview.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"
xcopy /Y "%QTDIR%\plugins\designer\qt3supportwidget.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"
xcopy /Y "%QTDIR%\plugins\designer\qwebview.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"

xcopy /Y "%QTDIR%\plugins\graphicssystem\qglgraphicssystemd4.dll" "%FINAL_PATH%\lib\qt4\plugins\graphicssystem\"
xcopy /Y "%QTDIR%\plugins\graphicssystem\qtracegraphicssystemd4.dll" "%FINAL_PATH%\lib\qt4\plugins\graphicssystem\"

xcopy /Y "%QTDIR%\plugins\iconengines\qsvgicond4.dll" "%FINAL_PATH%\lib\qt4\plugins\iconengines\"

xcopy /Y "%QTDIR%\plugins\imageformats\qgifd4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy /Y "%QTDIR%\plugins\imageformats\qicod4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy /Y "%QTDIR%\plugins\imageformats\qjpegd4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy /Y "%QTDIR%\plugins\imageformats\qmngd4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy /Y "%QTDIR%\plugins\imageformats\qsvgd4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy /Y "%QTDIR%\plugins\imageformats\qtiffd4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"

xcopy /Y "%QTDIR%\plugins\phonon_backend\phonon_ds9d4.dll" "%FINAL_PATH%\lib\qt4\plugins\phonon_backend\"

xcopy /Y "%QTDIR%\plugins\qmltooling\tcpserverd4.dll" "%FINAL_PATH%\lib\qt4\plugins\qmltooling\"

rem Add here your additional Qt sql drivers
xcopy /Y "%QTDIR%\plugins\sqldrivers\qsqlited4.dll" "%FINAL_PATH%\lib\qt4\plugins\qsqldrivers\"

goto create_nsis

:create_nsis
echo.
echo Creating project NSIS package ...
echo.
set STAGE=CREATING PLEXYDESK NSIS PACKAGE
makensis /V4 /DPRODUCT_PLATFORM=%PROJECT_PLATFORM% /DPRODUCT_VC_VERSION=%PROJECT_VC% /DPRODUCT_DEBUG=%PROJECT_DEBUG% /DPRODUCT_SOURCES_PATH=%GIT_PATH% /DPRODUCT_BIN_SOURCES_PATH=%FINAL_PATH% "%GIT_PATH%\build\dist\windows\make_nsis_installer.nsi"
if %ERRORLEVEL% NEQ 0 goto ERROR


:SUCCESS
echo.
echo All done.
echo.
echo You can find the prepared package for Windows in this folder:
echo.
echo %GIT_PATH%\INSTALLERS
echo.
goto END_INSTALL

:end_compiler_debug_mingw
echo.
echo Creating files with CMake ...
echo.
set STAGE=CREATING CMAKE FILES
cmake ../ -DCMAKE_INSTALL_PREFIX:STRING="%FINAL_PATH_FIXED%" -DCMAKE_BUILD_TYPE:STRING="%RELEASE_TYPE%" -G "%MINGW_VERSION%"
if %ERRORLEVEL% NEQ 0 goto ERROR

echo.
echo Building project ...
echo.
set STAGE=BUILDING THE PROJECT
mingw32-make install
if %ERRORLEVEL% NEQ 0 goto ERROR
goto finalize

:ERROR
echo.
echo There was an error in stage: %STAGE%
echo Look at the output above to get more info on this error.
echo.


:END_INSTALL
cd %CURRENT_PATH%
