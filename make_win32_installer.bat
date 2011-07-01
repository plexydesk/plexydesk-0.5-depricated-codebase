 
cmake -DCMAKE_INSTALL_PREFIX:STRING="c:/plexydesk" -G "Visual Studio 9 2008"

msbuild PlexyDesktop.sln /t:Build /p:Configuration=Release /V:n /nologo

msbuild /target:Build /p:Configuration=Release INSTALL.vcproj /V:n /nologo

xcopy %QTDIR%\bin\*.dll c:\plexydesk\bin

xcopy /E %QTDIR%\imports\*.*  c:\plexydesk\lib\qt4\imports\

echo "Following path is not a Mistake"

xcopy /E %QTDIR%\plugins\*.* c:\plexydesk\bin


ren delete unwanted files

del c:\plexydesk\bin\QtDesigner*
del c:\plexydesk\bin\QTHelp*
del c:\plexydesk\bin\QtCL*



"makensis" make_nsis_installer.nsi


