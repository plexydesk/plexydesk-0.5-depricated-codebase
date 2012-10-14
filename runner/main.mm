
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

    PlexyDesk::PluginLoader *loader =
            PlexyDesk::PluginLoader::getInstanceWithPrefix(
                QDir::toNativeSeparators(PlexyDesk::Config::getInstance()->plexydeskBasePath()  + QLatin1String("/share/plexy/ext/groups/")),
                QDir::toNativeSeparators(PlexyDesk::Config::getInstance()->plexydeskBasePath() + QLatin1String("/lib/plexyext/")));
    QString appIconPath = PlexyDesk::Config::getInstance()->plexydeskBasePath() +
        "/share/plexy/plexydesk.png";
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
        [[_desktopView window] makeKeyAndOrderFront:_desktopView];
    }

    return qtApp.exec();
}
