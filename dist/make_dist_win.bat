@echo off

rem Script for building and packing PlexyDesk on a Windows Platform
rem Authors:
rem     PhobosK <phobosk@kbfx.net>
rem     Dariusz Mikulski <dariusz.mikulski@gmail.com>
rem License: LGPLv3


rem release QT x86 configured with MS VS 2010 with the following options:
rem configure -release -opensource -qt-zlib -qt-libpng -qt-libmng -qt-libtiff -qt-libjpeg -plugin-sql-sqlite -plugin-sql-odbc -platform win32-msvc2010

rem release QT x64 configured with MS VS 2010 with the following options:
rem configure -release -opensource -qt-zlib -qt-libpng -qt-libmng -qt-libtiff -qt-libjpeg -plugin-sql-sqlite -plugin-sql-odbc


CLS
set CURRENT_PATH=%cd%


rem Make sure we do all operations from the GIT source path
cd /d %~dp0
cd ..
set GIT_PATH=%cd%


echo Welcome to the PlexyDesk MS Windows package creation tool
echo.
echo.
echo * Please make sure you have installed NSIS from http://nsis.sourceforge.net
echo   and added the path to makensis.exe to your PATH environment variable
echo.
echo.
echo * If you will use MinGW compiler, please make sure you have its bin folder
echo   added to your PATH environment variable (its installer sometimes fails with this step)
echo   AND PLEASE --- SET MINGW_BIN_DIR --- TO MINGW's BIN FOLDER FULL PATH
echo.
echo * Please make sure you have QTDIR environment variable set and it is
echo   pointing to the folder where QT is installed (debug or release QT)
echo.
echo.
echo * If you want to prepare a Debug release, please make sure you have a debug QT
echo   libraries compiled with the necessary debug flags
echo.
echo.
echo * If you will use MinGW compiler, please make sure you have a MinGW QT Release
echo   and this release bin path is preceding the bin path of an existing VC QT Release
echo   in your environment PATH or the build will fail
echo.
echo.

pause

echo.
echo 1. Quiet
echo 2. Minimal
echo 3. Normal
echo 4. Detailed
echo 5. Quit this tool
echo.

CHOICE /M "Please choose the debug level of this script [default is 1]: " /C 12345 /CS /D 1 /T 60 /N
if errorlevel 5 goto END_INSTALL
if errorlevel 4 goto detailed
if errorlevel 3 goto normal
if errorlevel 2 goto minimal
if errorlevel 1 goto quiet
echo.

:detailed
echo Setting script debug level to: Detailed
echo.
set COMPILER_DEBUG_LEVEL=d
set NSIS_DEBUG_LEVEL=4
set COPY_DEBUG_LEVEL=
goto end_script_debug

:normal
echo Setting script debug level to: Normal
echo.
set COMPILER_DEBUG_LEVEL=n
set NSIS_DEBUG_LEVEL=3
set COPY_DEBUG_LEVEL=
goto end_script_debug

:minimal
echo Setting script debug level to: Minimal
echo.
set COMPILER_DEBUG_LEVEL=m
set NSIS_DEBUG_LEVEL=2
set COPY_DEBUG_LEVEL=/Q
goto end_script_debug

:quiet
echo Setting script debug level to: Quiet
echo.
set COMPILER_DEBUG_LEVEL=q
set NSIS_DEBUG_LEVEL=1
set COPY_DEBUG_LEVEL=/Q


:end_script_debug
rem We better build out of the sources
IF EXIST "%GIT_PATH%\build" GOTO clean_build_folder
echo Creating a build folder ...
echo.
mkdir build

:clean_build_folder
rem Clean the build folder, so we do not get unneeded files in it
rem Note: del does not delete folders recursively, only files, leaving folders empty
echo Cleaning the build folder first from all files ...
echo (DISABLED)
rem set STAGE=INITIAL CLEANING OF BUILD FOLDER
rem del /Q /S /F build\*
rem if %ERRORLEVEL% NEQ 0 goto ERROR
echo.

echo.
echo Entering the build folder ...
echo.
cd build

rem The folder where all will be installed before packed
set FINAL_PATH_NAME=plexydesk-ready-build
set FINAL_PATH=%cd%\%FINAL_PATH_NAME%
set FINAL_PATH_FIXED=%FINAL_PATH:\=/%

rem First we create a folder where all Installers will go
IF EXIST "%GIT_PATH%\INSTALLERS" GOTO create_final_folder
echo Creating an INSTALLERS folder ...
echo.
mkdir INSTALLERS

:create_final_folder
IF EXIST "%FINAL_PATH%" GOTO clean_final_folder
mkdir "%FINAL_PATH%"

:clean_final_folder
rem Clean the final path folder, so we do not get unneeded files in it
echo Cleaning the final folder first from all files ...
set STAGE=INITIAL CLEANING OF TEMP INSTALL PATH
del /S /Q /F "%FINAL_PATH%"
if %ERRORLEVEL% NEQ 0 goto ERROR
echo.


echo.
echo 1. MS Visual Studio 10 2010
echo 2. MS Visual Studio 10 2010 (x64)
echo 3. MS Visual Studio 9 2008
echo 4. MS Visual Studio 9 2008 (x64)
echo 5. MinGW
echo 6. Quit this tool
echo.

CHOICE /M "Please choose which version of Compiler to use [default is 1]: " /C 123456 /CS /D 1 /T 60 /N
if errorlevel 6 goto END_INSTALL
if errorlevel 5 goto mingw
if errorlevel 4 goto vc9x64
if errorlevel 3 goto vc9
if errorlevel 2 goto vc10x64
if errorlevel 1 goto vc10
echo.

:mingw
echo Setting version to: MinGW Makefiles
echo.
set PROJECT_PLATFORM=x86
set PROJECT_VC=none
set MINGW_VERSION=MinGW Makefiles
goto end_compiler_type

:vc9x64
echo Setting version to: MS Visual Studio 9 2008 (x64)
echo.
set MSVC_VERSION=Visual Studio 9 2008 Win64
set PROJECT_EXT=vcproj
set PROJECT_PLATFORM=x64
set PROJECT_VC=VC9
set MINGW_VERSION=none
goto end_compiler_type

:vc9
echo Setting version to: MS Visual Studio 9 2008
echo.
set MSVC_VERSION=Visual Studio 9 2008
set PROJECT_EXT=vcproj
set PROJECT_PLATFORM=x86
set PROJECT_VC=VC9
set MINGW_VERSION=none
goto end_compiler_type

:vc10x64
echo Setting version to: MS Visual Studio 10 2010 (x64)
echo.
set MSVC_VERSION=Visual Studio 10 Win64
set PROJECT_EXT=vcxproj
set PROJECT_PLATFORM=x64
set PROJECT_VC=VC10
set MINGW_VERSION=none
goto end_compiler_type

:vc10
echo Setting version to: MS Visual Studio 10 2010
echo.
set MSVC_VERSION=Visual Studio 10
set PROJECT_EXT=vcxproj
set PROJECT_PLATFORM=x86
set PROJECT_VC=VC10
set MINGW_VERSION=none


:end_compiler_type
rem CMake prepares same types of builds for MinGW as for all others
echo.
echo 1. Release
echo 2. RelWithDebInfo
echo 3. MinSizeRel
echo 4. Debug
echo 5. Quit this tool
echo.

CHOICE /M "Please choose the release type of PlexyDesk to use [default is 1]: " /C 12345 /CS /D 1 /T 60 /N
if errorlevel 5 goto END_INSTALL
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
set PROJECT_DEBUG=none
goto end_rel_type

:rel
echo Setting release type to: Release
echo.
set RELEASE_TYPE=Release
set PROJECT_DEBUG=none


:end_rel_type
rem First check if QTDIR and MINGW_BIN_DIR exist. We need them for the xcopy
set STAGE=CHECKING VARIABLES VALIDITY
echo.
echo Checking if QTDIR and MINGW_BIN_DIR exist
echo.

if "%QTDIR%" NEQ "" goto check_mingw_var
echo You are missing the QTDIR in your PATH. Set it first. Aborting
goto ERROR

:check_mingw_var
if "%MINGW_VERSION%" EQU "none" goto clean_env_var
if "%MINGW_BIN_DIR%" NEQ "" goto clean_env_var
echo You are missing the MINGW_BIN_DIR in your PATH. Set it first. Aborting
goto ERROR

:clean_env_var
rem Then cleanup trailing slashes from env variables. xcopy doesn't like two backslashes
if "%MINGW_BIN_DIR:~-1,1%" EQU "\" set MINGW_BIN_DIR=%MINGW_BIN_DIR:~0,-1%
if "%QTDIR:~-1,1%" EQU "\" set QTDIR=%QTDIR:~0,-1%

echo Found QTDIR: %QTDIR%
echo Found MINGW_BIN_DIR: %MINGW_BIN_DIR%
echo.

rem Then ensure we have any debug/release QT libs
set STAGE=CHECKING EXISTENT QT FILES
if "%PROJECT_DEBUG%" NEQ "debug" goto check_qt_release_libs
if EXIST "%QTDIR%\lib\QtCored4.dll" goto begin_cmake
echo You have not compiled the debug QT libraries.
echo Check also if your QTDIR exists: %QTDIR%
echo Aborting
goto ERROR

:check_qt_release_libs
if EXIST "%QTDIR%\bin\QtCore4.dll" goto begin_cmake
echo You have not compiled the release QT libraries.
echo Check also if your QTDIR exists: %QTDIR%
echo Aborting
goto ERROR

:begin_cmake
echo.
echo Creating files with CMake ...
echo.

if "%MINGW_VERSION%" EQU "none" (set GENERATOR_VERSION=%MSVC_VERSION%) ELSE set GENERATOR_VERSION=%MINGW_VERSION%
echo Setting CMake Generator version to: %GENERATOR_VERSION%
echo.
set STAGE=CREATING CMAKE FILES
cmake ../ -DCMAKE_INSTALL_PREFIX="%FINAL_PATH_FIXED%" -DCMAKE_BUILD_TYPE="%RELEASE_TYPE%" -G "%GENERATOR_VERSION%"
if %ERRORLEVEL% NEQ 0 goto ERROR


echo.
echo Building project using %GENERATOR_VERSION%...
echo.
set STAGE=BUILDING THE PROJECT USING %GENERATOR_VERSION%

if "%MINGW_VERSION%" EQU "none" goto ms_build
"%MINGW_BIN_DIR%\mingw32-make.exe" install
if %ERRORLEVEL% NEQ 0 goto ERROR
goto finalize

:ms_build
msbuild PlexyDesktop.sln /v:%COMPILER_DEBUG_LEVEL% /t:Build /p:Configuration=%RELEASE_TYPE% /nologo
if %ERRORLEVEL% NEQ 0 goto ERROR


echo.
echo Installing project to a temp folder...
echo.
set STAGE=INSTALLING THE PROJECT TO A TEMP FOLDER
msbuild /target:Build /v:%COMPILER_DEBUG_LEVEL% /p:Configuration=%RELEASE_TYPE% INSTALL.%PROJECT_EXT% /nologo
if %ERRORLEVEL% NEQ 0 goto ERROR


:finalize
rem Default install of MinGW has the Qt*.dll in bin, handle this
if "%MINGW_VERSION%" EQU "none" (set QT_LIB_DIR_COPY=%QTDIR%\lib) ELSE set QT_LIB_DIR_COPY=%QTDIR%\bin

if "%PROJECT_DEBUG%" EQU "debug" goto copy_debug_qt_libs

echo.
echo Copying Release QT files to %FINAL_PATH% ...
echo.
set STAGE=COPYING RELEASE QT FILES

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\phonon4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\Qt3Support4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtCLucene4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtCore4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtDeclarative4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtDesigner4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtDesignerComponents4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtGui4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtHelp4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtMultimedia4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtNetwork4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtOpenGL4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtScript4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtScriptTools4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtSql4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtSvg4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtTest4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtWebKit4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtXml4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtXmlPatterns4.dll" "%FINAL_PATH%\lib\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\imports\Qt\labs\gestures\qmldir" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\gestures\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\imports\Qt\labs\gestures\qmlgesturesplugin.dll" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\gestures\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\imports\Qt\labs\particles\qmldir" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\particles\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\imports\Qt\labs\particles\qmlparticlesplugin.dll" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\particles\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\imports\QtWebKit\qmldir" "%FINAL_PATH%\lib\qt4\imports\QtWebKit\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\imports\QtWebKit\qmlwebkitplugin.dll" "%FINAL_PATH%\lib\qt4\imports\QtWebKit\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\accessible\qtaccessiblecompatwidgets4.dll" "%FINAL_PATH%\lib\qt4\plugins\accessible\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\accessible\qtaccessiblewidgets4.dll" "%FINAL_PATH%\lib\qt4\plugins\accessible\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\bearer\qgenericbearer4.dll" "%FINAL_PATH%\lib\qt4\plugins\bearer\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\bearer\qnativewifibearer4.dll" "%FINAL_PATH%\lib\qt4\plugins\bearer\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\codecs\qcncodecs4.dll" "%FINAL_PATH%\lib\qt4\plugins\codecs\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\codecs\qjpcodecs4.dll" "%FINAL_PATH%\lib\qt4\plugins\codecs\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\codecs\qkrcodecs4.dll" "%FINAL_PATH%\lib\qt4\plugins\codecs\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\codecs\qtwcodecs4.dll" "%FINAL_PATH%\lib\qt4\plugins\codecs\"

rem Only release dll's
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\designer\phononwidgets.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\designer\qaxwidget.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\designer\qdeclarativeview.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\designer\qt3supportwidgets.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\designer\qwebview.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\graphicssystems\qglgraphicssystem4.dll" "%FINAL_PATH%\lib\qt4\plugins\graphicssystem\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\graphicssystems\qtracegraphicssystem4.dll" "%FINAL_PATH%\lib\qt4\plugins\graphicssystem\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\iconengines\qsvgicon4.dll" "%FINAL_PATH%\lib\qt4\plugins\iconengines\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\imageformats\qgif4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\imageformats\qico4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\imageformats\qjpeg4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\imageformats\qmng4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\imageformats\qsvg4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\imageformats\qtiff4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\phonon_backend\phonon_ds94.dll" "%FINAL_PATH%\lib\qt4\plugins\phonon_backend\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\qmltooling\tcpserver4.dll" "%FINAL_PATH%\lib\qt4\plugins\qmltooling\"

rem Add here your additional QT SQL drivers
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\sqldrivers\qsqlite4.dll" "%FINAL_PATH%\lib\qt4\plugins\qsqldrivers\"

goto cleanup_install


:copy_debug_qt_libs
echo.
echo Copying Debug QT files to %FINAL_PATH% ...
echo.
set STAGE=COPYING DEBUG QT FILES

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\phonond4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\Qt3Supportd4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtCLucened4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtCored4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtDeclaratived4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtDesignerd4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtDesignerComponentsd4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtGuid4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtHelpd4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtMultimediad4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtNetworkd4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtOpenGLd4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtScriptd4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtScriptToolsd4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtSqld4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtSvgd4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtTestd4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtWebKitd4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtXmld4.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QT_LIB_DIR_COPY%\QtXmlPatternsd4.dll" "%FINAL_PATH%\lib\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\imports\Qt\labs\gestures\qmldir" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\gestures\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\imports\Qt\labs\gestures\qmlgesturesplugind.dll" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\gestures\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\imports\Qt\labs\particles\qmldir" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\particles\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\imports\Qt\labs\particles\qmlparticlesplugind.dll" "%FINAL_PATH%\lib\qt4\imports\Qt\labs\particles\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\imports\QtWebKit\qmldir" "%FINAL_PATH%\lib\qt4\imports\QtWebKit\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\imports\QtWebKit\qmlwebkitplugind.dll" "%FINAL_PATH%\lib\qt4\imports\QtWebKit\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\accessible\qtaccessiblecompatwidgetsd4.dll" "%FINAL_PATH%\lib\qt4\plugins\accessible\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\accessible\qtaccessiblewidgetsd4.dll" "%FINAL_PATH%\lib\qt4\plugins\accessible\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\bearer\qgenericbearerd4.dll" "%FINAL_PATH%\lib\qt4\plugins\bearer\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\bearer\qnativewifibearerd4.dll" "%FINAL_PATH%\lib\qt4\plugins\bearer\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\codecs\qcncodecsd4.dll" "%FINAL_PATH%\lib\qt4\plugins\codecs\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\codecs\qjpcodecsd4.dll" "%FINAL_PATH%\lib\qt4\plugins\codecs\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\codecs\qkrcodecsd4.dll" "%FINAL_PATH%\lib\qt4\plugins\codecs\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\codecs\qtwcodecsd4.dll" "%FINAL_PATH%\lib\qt4\plugins\codecs\"

rem Only release dll's
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\designer\phononwidgets.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\designer\qaxwidget.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\designer\qdeclarativeview.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\designer\qt3supportwidgets.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\designer\qwebview.dll" "%FINAL_PATH%\lib\qt4\plugins\designer\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\graphicssystems\qglgraphicssystemd4.dll" "%FINAL_PATH%\lib\qt4\plugins\graphicssystem\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\graphicssystems\qtracegraphicssystemd4.dll" "%FINAL_PATH%\lib\qt4\plugins\graphicssystem\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\iconengines\qsvgicond4.dll" "%FINAL_PATH%\lib\qt4\plugins\iconengines\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\imageformats\qgifd4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\imageformats\qicod4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\imageformats\qjpegd4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\imageformats\qmngd4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\imageformats\qsvgd4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\imageformats\qtiffd4.dll" "%FINAL_PATH%\lib\qt4\plugins\imageformats\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\phonon_backend\phonon_ds9d4.dll" "%FINAL_PATH%\lib\qt4\plugins\phonon_backend\"

xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\qmltooling\tcpserverd4.dll" "%FINAL_PATH%\lib\qt4\plugins\qmltooling\"

rem Add here your additional QT SQL drivers
xcopy %COPY_DEBUG_LEVEL% /R /Y "%QTDIR%\plugins\sqldrivers\qsqlited4.dll" "%FINAL_PATH%\lib\qt4\plugins\qsqldrivers\"


:cleanup_install
echo.
echo Deleting unnecessary files ...
echo.
set STAGE=DELETING UNNECESSARRY FILES
del /Q /S /F "%FINAL_PATH%\*.a"
del /Q /S /F "%FINAL_PATH%\*.lib"

rem Adding MinGW dist files
if "%MINGW_VERSION%" EQU "none" goto create_nsis
echo.
echo Adding the neccessary MinGW dist files ...
echo.
set STAGE=ADDING MINGW FILES
xcopy %COPY_DEBUG_LEVEL% /R /Y "%MINGW_BIN_DIR%\libgcc_s_dw*.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%MINGW_BIN_DIR%\mingwm*.dll" "%FINAL_PATH%\lib\"
xcopy %COPY_DEBUG_LEVEL% /R /Y "%MINGW_BIN_DIR%\libstdc*.dll" "%FINAL_PATH%\lib\"

:create_nsis
echo.
echo Creating project NSIS package ...
echo.
set STAGE=CREATING PLEXYDESK NSIS PACKAGE
makensis /V%NSIS_DEBUG_LEVEL% /DPRODUCT_PLATFORM=%PROJECT_PLATFORM% /DPRODUCT_VC_VERSION=%PROJECT_VC% /DPRODUCT_DEBUG=%PROJECT_DEBUG% /DPRODUCT_SOURCES_PATH=%GIT_PATH% /DPRODUCT_BIN_SOURCES_PATH=%FINAL_PATH% "%GIT_PATH%\build\dist\windows\make_nsis_installer.nsi"
if %ERRORLEVEL% NEQ 0 goto ERROR


:SUCCESS
echo.
echo All done.
echo.
echo You can find the prepared PlexyDesk install package for Windows in this folder:
echo.
echo %GIT_PATH%\INSTALLERS
echo.
dir "%GIT_PATH%\INSTALLERS"
goto END_INSTALL


:ERROR
echo.
echo There was an error at stage: %STAGE%
echo Look at the output above to get more info on this error.
echo.


:END_INSTALL
cd "%CURRENT_PATH%"
