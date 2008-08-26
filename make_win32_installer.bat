cmake -DCMAKE_INSTALL_PREFIX:STRING="c:\plexydesk\" -G "Visual Studio 9 2008"
msbuild PlexyDesktop.sln /t:Build /p:Configuration=Release

mkdir c:\plexydesk
mkdir c:\plexydesk
mkdir c:\plexydesk\bin
mkdir c:\plexydesk\bin\plugins
mkdir c:\plexydesk\bin\plugins\imageformats

vcbuild INSTALL.vcproj "Release|Win32"

del /F/S/Q c:\plexydesk\include\qimageblitz\*.*
rmdir /S/Q c:\plexydesk\include\qimageblitz
rmdir /S/Q c:\plexydesk\include
del /F/S/Q c:\plexydesk\*.lib

xcopy %QTDIR%\bin\QtSvg4.dll c:\plexydesk\bin
xcopy %QTDIR%\bin\QtOpenGL4.dll c:\plexydesk\bin
xcopy %QTDIR%\bin\QtNetwork4.dll c:\plexydesk\bin
xcopy %QTDIR%\bin\QtGui4.dll c:\plexydesk\bin
xcopy %QTDIR%\bin\QtCore4.dll c:\plexydesk\bin

xcopy %QTDIR%\plugins\imageformats\qtiff4.dll c:\plexydesk\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qsvg4.dll c:\plexydesk\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qmng4.dll c:\plexydesk\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qjpeg4.dll c:\plexydesk\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qico4.dll c:\plexydesk\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qgif4.dll c:\plexydesk\bin\plugins\imageformats

"C:\Program Files\NSIS\makensis" installer.nsi

rmdir /S/Q c:\plexydesk\