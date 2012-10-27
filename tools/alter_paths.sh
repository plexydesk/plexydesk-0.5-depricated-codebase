#before you run the script run make install, then run macdeply 
rm -rf ~/Applications/plexydesk.app/
make install
macdeployqt ~/Applications/plexydesk.app/bin/plexydesk.app/

for x in ~/Applications/plexydesk.app/bin/plexydesk.app/Contents/MacOS/plexydesk
do
install_name_tool -change libplexydeskcore.dylib  @executable_path/../Frameworks/libplexydeskcore.dylib $x
install_name_tool -change libplexydeskuicore.dylib  @executable_path/../Frameworks/libplexydeskuicore.dylib $x
install_name_tool -change libplexymime.dylib @executable_path/../Frameworks/libplexymime.dylib $x
install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore $x
install_name_tool -change QtXmlPatterns.framework/Versions/4/QtXmlPatterns @executable_path/../Frameworks/QtXmlPatterns.framework/Versions/4/QtXmlPatterns $x
install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui $x
install_name_tool -change QtOpenGL.framework/Versions/4/QtOpenGL  @executable_path/../Frameworks/QtOpenGL.framework/Versions/4/QtOpenGL $x
install_name_tool -change QtSvg.framework/Versions/4/QtSvg @executable_path/../Frameworks/QtSvg.framework/Versions/4/QtSvg $x
install_name_tool -change QtNetwork.framework/Versions/4/QtNetwork @executable_path/../Frameworks/QtNetwork.framework/Versions/4/QtNetwork $x
install_name_tool -change QtDeclarative.framework/Versions/4/QtDeclarative @executable_path/../Frameworks/QtDeclarative.framework/Versions/4/QtDeclarative $x
done

cp -v ~/Applications/plexydesk.app/lib/lib* ~/Applications/plexydesk.app/bin/plexydesk.app/Contents/Frameworks/

for x in ~/Applications/plexydesk.app/bin/plexydesk.app/Contents/Frameworks/lib*
do
install_name_tool -change libplexydeskcore.dylib  @executable_path/../Frameworks/libplexydeskcore.dylib $x
install_name_tool -change libplexydeskuicore.dylib  @executable_path/../Frameworks/libplexydeskuicore.dylib $x
install_name_tool -change libplexymime.dylib @executable_path/../Frameworks/libplexymime.dylib $x
install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore $x
install_name_tool -change QtXmlPatterns.framework/Versions/4/QtXmlPatterns @executable_path/../Frameworks/QtXmlPatterns.framework/Versions/4/QtXmlPatterns $x
install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui $x
install_name_tool -change QtOpenGL.framework/Versions/4/QtOpenGL  @executable_path/../Frameworks/QtOpenGL.framework/Versions/4/QtOpenGL $x
install_name_tool -change QtSvg.framework/Versions/4/QtSvg @executable_path/../Frameworks/QtSvg.framework/Versions/4/QtSvg $x
install_name_tool -change QtNetwork.framework/Versions/4/QtNetwork @executable_path/../Frameworks/QtNetwork.framework/Versions/4/QtNetwork $x
install_name_tool -change QtDeclarative.framework/Versions/4/QtDeclarative @executable_path/../Frameworks/QtDeclarative.framework/Versions/4/QtDeclarative $x
done


for x in ~/Applications/plexydesk.app/lib/plexyext/lib*
do
install_name_tool -change libplexydeskcore.dylib  @executable_path/../Frameworks/libplexydeskcore.dylib $x
install_name_tool -change libplexydeskuicore.dylib  @executable_path/../Frameworks/libplexydeskuicore.dylib $x
install_name_tool -change libplexymime.dylib @executable_path/../Frameworks/libplexymime.dylib $x
install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore $x
install_name_tool -change QtXmlPatterns.framework/Versions/4/QtXmlPatterns @executable_path/../Frameworks/QtXmlPatterns.framework/Versions/4/QtXmlPatterns $x
install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui $x
install_name_tool -change QtOpenGL.framework/Versions/4/QtOpenGL  @executable_path/../Frameworks/QtOpenGL.framework/Versions/4/QtOpenGL $x
install_name_tool -change QtSvg.framework/Versions/4/QtSvg @executable_path/../Frameworks/QtSvg.framework/Versions/4/QtSvg $x
install_name_tool -change QtNetwork.framework/Versions/4/QtNetwork @executable_path/../Frameworks/QtNetwork.framework/Versions/4/QtNetwork $x
install_name_tool -change QtDeclarative.framework/Versions/4/QtDeclarative @executable_path/../Frameworks/QtDeclarative.framework/Versions/4/QtDeclarative $x
done


##echo "Imports = imports" >> ~/Applications/plexydesk.app/bin/plexydesk.app/Contents/Resources/qt.conf
mkdir ~/Applications/plexydesk.app/bin/plexydesk.app/Contents/imports

cp -vr $QTDIR/imports/* ~/Applications/plexydesk.app/bin/plexydesk.app/Contents/imports/

for x in  ~/Applications/plexydesk.app/bin/plexydesk.app/Contents/imports/Qt/labs/FolderListModel/lib*
do
install_name_tool -change libplexydeskcore.dylib  @executable_path/../Frameworks/libplexydeskcore.dylib $x
install_name_tool -change libplexydeskuicore.dylib  @executable_path/../Frameworks/libplexydeskuicore.dylib $x
install_name_tool -change libplexymime.dylib @executable_path/../Frameworks/libplexymime.dylib $x
install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore $x
install_name_tool -change QtXmlPatterns.framework/Versions/4/QtXmlPatterns @executable_path/../Frameworks/QtXmlPatterns.framework/Versions/4/QtXmlPatterns $x
install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui $x
install_name_tool -change QtOpenGL.framework/Versions/4/QtOpenGL  @executable_path/../Frameworks/QtOpenGL.framework/Versions/4/QtOpenGL $x
install_name_tool -change QtSvg.framework/Versions/4/QtSvg @executable_path/../Frameworks/QtSvg.framework/Versions/4/QtSvg $x
install_name_tool -change QtNetwork.framework/Versions/4/QtNetwork @executable_path/../Frameworks/QtNetwork.framework/Versions/4/QtNetwork $x
install_name_tool -change QtDeclarative.framework/Versions/4/QtDeclarative @executable_path/../Frameworks/QtDeclarative.framework/Versions/4/QtDeclarative $x
install_name_tool -change QtSql.framework/Versions/4/QtSql @executable_path/../Frameworks/QtDeclarative.framework/Versions/4/QtDeclarative $x
done

mv ~/Applications/plexydesk.app/bin/plexydesk.app/Contents ~/Applications/plexydesk.app/
mv ~/Applications/plexydesk.app/lib ~/Applications/plexydesk.app/Contents/
mv ~/Applications/plexydesk.app/share ~/Applications/plexydesk.app/Contents/

macdeployqt ~/Applications/plexydesk.app/

echo Done!
