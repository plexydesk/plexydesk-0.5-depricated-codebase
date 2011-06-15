cmake -DCMAKE_INSTALL_PREFIX:STRING="c:/plexydesk" -G "Visual Studio 9 2008"
msbuild PlexyDesktop.sln /t:Build /p:Configuration=Release /V:m

mkdir plexydesk
mkdir plexydesk\bin
mkdir plexydesk\bin\plugins
mkdir plexydesk\bin\plugins\imageformats

vcbuild INSTALL.vcproj "Release|Win32"


del /F/S/Q plexydesk\include\qimageblitz\*.*
rmdir /S/Q plexydesk\include\qimageblitz
rmdir /S/Q plexydesk\include
del /F/S/Q plexydesk\*.lib

xcopy %QTDIR%\bin\QtSvg4.dll plexydesk\bin
xcopy %QTDIR%\bin\QtOpenGL4.dll plexydesk\bin
xcopy %QTDIR%\bin\QtNetwork4.dll plexydesk\bin
xcopy %QTDIR%\bin\QtGui4.dll plexydesk\bin
xcopy %QTDIR%\bin\QtCore4.dll plexydesk\bin

xcopy %QTDIR%\plugins\imageformats\qtiff4.dll plexydesk\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qsvg4.dll plexydesk\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qmng4.dll plexydesk\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qjpeg4.dll plexydesk\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qico4.dll plexydesk\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qgif4.dll plexydesk\bin\plugins\imageformats

"makensis" installer.nsi