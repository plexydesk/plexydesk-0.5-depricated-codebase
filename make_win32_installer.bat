cmake -DCMAKE_INSTALL_PREFIX:STRING="c:/plexydesk" -G "Visual Studio 9 2008"
msbuild PlexyDesktop.sln /t:Build /p:Configuration=Release /V:d /nologo

mkdir plexydesk
mkdir plexydesk\bin
mkdir plexydesk\bin\plugins
mkdir plexydesk\bin\plugins\imageformats

ren vcbuild INSTALL.vcproj "Release|Win32"

msbuild /target:Build /p:Configuration=Release INSTALL.vcproj /V:d /nologo

xcopy %QTDIR%\bin\*.dll c:\plexydesk\bin

xcopy /E %QTDIR%\imports\*.*  c:\plexydesk\lib\qt4\imports\


xcopy /E %QTDIR%\plugins\*.* c:\plexydesk\bin\

"makensis" installer.nsi


