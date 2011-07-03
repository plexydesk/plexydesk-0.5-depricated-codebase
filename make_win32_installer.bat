@echo off

rem QT x86 configured with MS VS 2010 with the folowing options:
rem configure -release -opensource -qt-zlib -qt-libpng -qt-libmng -qt-libtiff -qt-libjpeg -platform win32-msvc2010

rem QT x64 configured with MS VS 2010 with the folowing options:
rem configure -release -opensource -qt-zlib -qt-libpng -qt-libmng -qt-libtiff -qt-libjpeg


rem We better build out of the sources
IF EXIST "%cd%\build" GOTO go_to_build
mkdir build

:go_to_build
cd build

rem The directory where all will be installed before packed
set FINAL_PATH_NAME=plexydesk-ready-build
set FINAL_PATH=%cd%\%FINAL_PATH_NAME%
set FINAL_PATH_FIXED=%FINAL_PATH:\=/%

IF EXIST "%FINAL_PATH%" GOTO go_on
mkdir "%FINAL_PATH%"

:go_on


echo 1. MS Visual Studio 10 2010
echo 2. MS Visual Studio 10 2010 (x64)
echo 3. MS Visual Studio 9 2008
echo 4. MS Visual Studio 9 2008 (x64)

CHOICE /M "Please choose which version of MS VC to use [default is 1]: " /C 1234 /CS /D 1 /T 20 /N
if errorlevel 4 goto vc9x64
if errorlevel 3 goto vc9
if errorlevel 2 goto vc10x64
if errorlevel 1 goto vc10

:vc9x64
echo Setting version to: MS Visual Studio 9 2008 (x64)
set MSVC_VERSION=Visual Studio 9 2008 Win64
set PROJECT_EXT=vcproj
goto end_compiler_type

:vc9
echo Setting version to: MS Visual Studio 9 2008
set MSVC_VERSION=Visual Studio 9 2008
set PROJECT_EXT=vcproj
goto end_compiler_type

:vc10x64
echo Setting version to: MS Visual Studio 10 2010 (x64)
set MSVC_VERSION=Visual Studio 10 Win64
set PROJECT_EXT=vcxproj
goto end_compiler_type

:vc10
echo Setting version to: MS Visual Studio 10 2010
set MSVC_VERSION=Visual Studio 10
set PROJECT_EXT=vcxproj
:end_compiler_type


echo 1. Release
echo 2. RelWithDebInfo
echo 3. MinSizeRel
echo 4. Debug

CHOICE /M "Please choose the release type to use [default is 1]: " /C 1234 /CS /D 1 /T 20 /N
if errorlevel 4 goto dbg
if errorlevel 3 goto msr
if errorlevel 2 goto rdbg
if errorlevel 1 goto rel

:dbg
echo Setting release type to: Debug
set RELEASE_TYPE=Debug
goto end_rel_type

:msr
echo Setting release type to: MinSizeRel
set RELEASE_TYPE=MinSizeRel
goto end_rel_type

:rdbg
echo Setting release type to: RelWithDebInfo
set RELEASE_TYPE=RelWithDebInfo
goto end_rel_type

:rel
echo Setting release type to: Release
set RELEASE_TYPE=Release
:end_rel_type


echo 1. Quiet
echo 2. Minimal
echo 3. Normal
echo 4. Detailed

CHOICE /M "Please choose the debug level of the compilation [default is 1]: " /C 1234 /CS /D 1 /T 20 /N
if errorlevel 4 goto detailed
if errorlevel 3 goto normal
if errorlevel 2 goto minimal
if errorlevel 1 goto quiet

:detailed
echo Setting compilation debug level to: Detailed
set COMPILER_DEBUG_LEVEL=d
goto end_compiler_debug

:normal
echo Setting compilation debug level to: Normal
set COMPILER_DEBUG_LEVEL=n
goto end_compiler_debug

:minimal
echo Setting compilation debug level to: Minimal
set COMPILER_DEBUG_LEVEL=m
goto end_compiler_debug

:quiet
echo Setting compilation debug level to: Quiet
set COMPILER_DEBUG_LEVEL=q
:end_compiler_debug


echo Creating files with CMake ...
cmake ../ -DCMAKE_INSTALL_PREFIX:STRING="%FINAL_PATH_FIXED%" -DCMAKE_BUILD_TYPE:STRING="%RELEASE_TYPE%" -G "%MSVC_VERSION%"

echo Building project ...
msbuild PlexyDesktop.sln /v:%COMPILER_DEBUG_LEVEL% /t:Build /p:Configuration=%RELEASE_TYPE% /nologo

echo Installing project ...
msbuild /target:Build /v:%COMPILER_DEBUG_LEVEL% /p:Configuration=%RELEASE_TYPE% INSTALL.%PROJECT_EXT% /nologo

echo Copying QT files to %FINAL_PATH% ...
xcopy /Q /Y "%QTDIR%\bin\*.dll" "%FINAL_PATH%\bin\"
xcopy /E /Q /Y "%QTDIR%\imports\*.*"  "%FINAL_PATH%\lib\qt4\imports\"
xcopy /E /Q /Y "%QTDIR%\plugins\*.*" "%FINAL_PATH%\bin\"

echo Deleting unnecessary files ...
del /Q /F "%FINAL_PATH%\bin\QtDesigner*"
del /Q /F "%FINAL_PATH%\bin\QTHelp*"
del /Q /F "%FINAL_PATH%\bin\QtCL*"

rem echo Creating project package ...
rem "makensis" make_nsis_installer.nsi

:END_INSTALL
cd ..
