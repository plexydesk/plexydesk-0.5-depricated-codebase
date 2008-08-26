cmake -DCMAKE_INSTALL_PREFIX:STRING="c:\installer" -G "Visual Studio 9 2008"
msbuild PlexyDesktop.sln /t:Build /p:Configuration=Release

mkdir c:\installer
mkdir c:\installer
mkdir c:\installer\bin
mkdir c:\installer\bin\plugins
mkdir c:\installer\bin\plugins\imageformats

vcbuild INSTALL.vcproj "Release|Win32"

del /F/S/Q c:\installer\include\qimageblitz\*.*
rmdir /S/Q c:\installer\include\qimageblitz
rmdir /S/Q c:\installer\include
del /F/S/Q c:\installer\*.lib


xcopy %QTDIR%\bin\QtSvg4.dll c:\installer\bin
xcopy %QTDIR%\bin\QtOpenGL4.dll c:\installer\bin
xcopy %QTDIR%\bin\QtNetwork4.dll c:\installer\bin
xcopy %QTDIR%\bin\QtGui4.dll c:\installer\bin
xcopy %QTDIR%\bin\QtCore4.dll c:\installer\bin

xcopy %QTDIR%\plugins\imageformats\qtiff4.dll c:\installer\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qsvg4.dll c:\installer\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qmng4.dll c:\installer\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qjpeg4.dll c:\installer\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qico4.dll c:\installer\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qgif4.dll c:\installer\bin\plugins\imageformats

"C:\Program Files\NSIS\makensis" installer.nsi

rem rmdir /S/Q c:\installer