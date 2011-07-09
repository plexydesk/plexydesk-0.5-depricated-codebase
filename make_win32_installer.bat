@echo off

rem Script for building and packing PlexyDesk on Windows Platform
rem Author: PhobosK <phobosk@kbfx.net>
rem License: LGPLv3

CLS
set CURRENT_PATH=%cd%

rem QT x86 configured with MS VS 2010 with the folowing options:
rem configure -release -opensource -qt-zlib -qt-libpng -qt-libmng -qt-libtiff -qt-libjpeg -platform win32-msvc2010

rem QT x64 configured with MS VS 2010 with the folowing options:
rem configure -release -opensource -qt-zlib -qt-libpng -qt-libmng -qt-libtiff -qt-libjpeg


rem First we create a folder where all Installers will go
IF EXIST "%CURRENT_PATH%\INSTALLERS" GOTO create_build
echo Creating an INSTALLERS folder ...
echo.
mkdir INSTALLERS

create_build:
rem We better build out of the sources
IF EXIST "%CURRENT_PATH%\build" GOTO go_to_build
echo Creating a build folder ...
echo.
mkdir build

:go_to_build
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
echo.

CHOICE /M "Please choose which version of MS VC to use [default is 1]: " /C 1234 /CS /D 1 /T 20 /N
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
:end_compiler_type


echo.
echo 1. Release
echo 2. RelWithDebInfo
echo 3. MinSizeRel
echo 4. Debug
echo.

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
goto end_rel_type

:msr
echo Setting release type to: MinSizeRel
echo.
set RELEASE_TYPE=MinSizeRel
goto end_rel_type

:rdbg
echo Setting release type to: RelWithDebInfo
echo.
set RELEASE_TYPE=RelWithDebInfo
goto end_rel_type

:rel
echo Setting release type to: Release
echo.
set RELEASE_TYPE=Release
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
cmake ../ -DCMAKE_INSTALL_PREFIX:STRING="%FINAL_PATH_FIXED%" -DCMAKE_BUILD_TYPE:STRING="%RELEASE_TYPE%" -G "%MSVC_VERSION%"

echo.
echo Building project ...
echo.
msbuild PlexyDesktop.sln /v:%COMPILER_DEBUG_LEVEL% /t:Build /p:Configuration=%RELEASE_TYPE% /nologo

echo.
echo Installing project ...
echo.
msbuild /target:Build /v:%COMPILER_DEBUG_LEVEL% /p:Configuration=%RELEASE_TYPE% INSTALL.%PROJECT_EXT% /nologo

echo.
echo Copying QT files to %FINAL_PATH% ...
echo.
xcopy /Q /Y "%QTDIR%\bin\*.dll" "%FINAL_PATH%\lib\qt4\bin\"
xcopy /E /Q /Y "%QTDIR%\imports\*.*"  "%FINAL_PATH%\lib\qt4\imports\"
xcopy /E /Q /Y "%QTDIR%\plugins\*.*" "%FINAL_PATH%\lib\qt4\plugins\"

echo.
echo Deleting unnecessary QT files ...
echo.
del /Q /F "%FINAL_PATH%\lib\qt4\bin\QtDesigner*"
del /Q /F "%FINAL_PATH%\lib\qt4\bin\QTHelp*"
del /Q /F "%FINAL_PATH%\lib\qt4\bin\QtCL*"

echo.
echo Creating project NSIS package ...
echo.
"makensis"   /DPRODUCT_PLATFORM=%PROJECT_PLATFORM% \
             /DPRODUCT_VC_VERSION=%PROJECT_VC% \
             /DPRODUCT_SOURCES_PATH=%CURRENT_PATH% \
             /DPRODUCT_BIN_SOURCES_PATH=%FINAL_PATH% \
             "%CURRENT_PATH%\build\windows\make_nsis_installer.nsi"

:END_INSTALL

echo.
echo All done.
echo.
echo You can find the prepared package for Windows in this folder:
echo.
echo %CURRENT_PATH%\INSTALLERS
echo.

cd ..
