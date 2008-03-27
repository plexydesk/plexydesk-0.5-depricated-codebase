cmake -DCMAKE_INSTALL_PREFIX:STRING="installer" -G "Visual Studio 9 2008"
msbuild PlexyDesktop.sln /t:Build /p:Configuration=Release

mkdir .\installer
mkdir .\installer\installer
mkdir .\installer\installer\bin
mkdir .\installer\installer\bin\plugins
mkdir .\installer\installer\bin\plugins\imageformats

del /F/S/Q .\installer\installer\*.lib

xcopy %QTDIR%\bin\QtSvg4.dll .\installer\installer\bin
xcopy %QTDIR%\bin\QtOpenGL4.dll .\installer\installer\bin
xcopy %QTDIR%\bin\QtNetwork4.dll .\installer\installer\bin
xcopy %QTDIR%\bin\QtGui4.dll .\installer\installer\bin
xcopy %QTDIR%\bin\QtCore4.dll .\installer\installer\bin

xcopy %QTDIR%\plugins\imageformats\qtiff4.dll .\installer\installer\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qsvg4.dll .\installer\installer\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qmng4.dll .\installer\installer\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qjpeg4.dll .\installer\installer\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qico4.dll .\installer\installer\bin\plugins\imageformats
xcopy %QTDIR%\plugins\imageformats\qgif4.dll .\installer\installer\bin\plugins\imageformats

"C:\Program Files\NSIS\makensis" installer.nsi

rmdir /S/Q installer