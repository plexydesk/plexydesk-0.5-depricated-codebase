
#include <config.h>

#import <Cocoa/Cocoa.h>
#include <QtGui/QtGui>
#include <QtGui/qmacnativewidget_mac.h>
#include <QtCore>
#include <QIcon>

#include <plexy.h>
#include <desktopbaseui.h>
#include <plexyconfig.h>
#include <pluginloader.h>
#include <debug.h>


int main(int argc, char **argv)
{
    QApplication qtApp(argc, argv);

    NSLog(@"Start PlexyDesk :MacUI \n");

    CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef, kCFURLPOSIXPathStyle);
    const char *pathPtr = CFStringGetCStringPtr(macPath,CFStringGetSystemEncoding());
    qDebug("Path = %s", pathPtr);
    CFRelease(appUrlRef);
    CFRelease(macPath);

    PlexyDesk::PluginLoader *loader =
            PlexyDesk::PluginLoader::getInstanceWithPrefix(
                QDir::toNativeSeparators(QLatin1String(pathPtr)  + QLatin1String("/Contents/share/plexy/ext/groups/")),
                QDir::toNativeSeparators(QLatin1String(pathPtr)  + QLatin1String("/Contents/lib/plexyext/")));

    QString appIconPath =  QLatin1String(pathPtr) +
        "/Contents/share/plexy/plexydesk.png";
    QIcon appIcon = QIcon(QDir::toNativeSeparators(appIconPath));
    qtApp.setWindowIcon(appIcon);
    qtApp.setApplicationName(QString(PLEXYNAME));

    //PlexyDesk Base Desktop View
    QSharedPointer<DesktopBaseUi> ui = QSharedPointer<DesktopBaseUi>(new DesktopBaseUi(), &QObject::deleteLater);

    Q_FOREACH (QGraphicsView *child_desktop, ui->views()) {
        NSView *_desktopView = reinterpret_cast<NSView *>(child_desktop->winId());
        [[_desktopView window] setCollectionBehavior:NSWindowCollectionBehaviorCanJoinAllSpaces];
        [[_desktopView window] setHasShadow:NO];
        [[_desktopView window] setLevel:kCGDesktopIconWindowLevel + 1];
        //[[_desktopView window] makeKeyAndOrderFront:_desktopView];
    }

    //hack for mac
    //TODO: Fix this!
    qtApp.setStyleSheet(QString("QScrollBar:vertical{ border: 2px solid grey; background: #32CC99; height: 15px; margin: 0px 20px 0 20px; }"));

    return qtApp.exec();
}
