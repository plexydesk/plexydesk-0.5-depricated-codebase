
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

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSLog(@"Start PlexyDesk :MacUI \n");

    QString appIconPath = PlexyDesk::Config::getInstance()->plexydeskBasePath() +
        "/share/plexy/plexydesk.png";
    QIcon appIcon = QIcon(QDir::toNativeSeparators(appIconPath));
    qtApp.setWindowIcon(appIcon);
    qtApp.setApplicationName(QString(PLEXYNAME));


    NSWindow *macNSWindow =
            [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, qtApp.desktop()->width() , qtApp.desktop()->height())
            styleMask:NSBorderlessWindowMask
            backing:NSBackingStoreBuffered defer:NO];

    QMacNativeWidget *nativeWidget = new QMacNativeWidget();
    nativeWidget->move(0, 0);
    nativeWidget->setPalette(QPalette(Qt::transparent));
    nativeWidget->setAutoFillBackground(true);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    //QPushButton *pushButton = new QPushButton("An Embedded Qt Button!", nativeWidget);
    DesktopBaseUi * ui = new DesktopBaseUi(nativeWidget);
    ui->setPalette(QPalette(Qt::transparent));
    ui->setAutoFillBackground(true);
    ui->setAttribute(Qt::WA_LayoutUsesWidgetRect); // Don't use the layout rect calculated from QMacStyle.
    layout->addWidget(ui);
    nativeWidget->setLayout(layout);

#ifdef Q_WS_MAC
   // PlexyDesk::Config::getInstance()->setOpenGL(true);
#endif
    PlexyDesk::PluginLoader *loader = PlexyDesk::PluginLoader::getInstance();
#ifdef Q_WS_MAC
    loader->setPluginPrefix(QDir::toNativeSeparators(PlexyDesk::Config::getInstance()->plexydeskBasePath() + "/lib/plexyext/lib"));
#endif
    loader->scanDisk();

    // Set this to false if you need a "close to tray" functionality when systray exists
    QApplication::setQuitOnLastWindowClosed(true);

    NSView *nativeWidgetView = reinterpret_cast<NSView *>(nativeWidget->winId());
    NSView *contentView = [macNSWindow contentView];


    [contentView setAutoresizesSubviews:YES];
    [nativeWidgetView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
    [nativeWidgetView setAutoresizesSubviews:YES];
    // Add the nativeWidget to the window.
    [contentView addSubview:nativeWidgetView positioned:NSWindowAbove relativeTo:nil];

    [macNSWindow setBackgroundColor:[NSColor clearColor]], [macNSWindow setOpaque:NO];
    NSView *uiView = reinterpret_cast<NSView *>(ui->winId());
    nativeWidget->resize(ui->size());
    [uiView setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];

    nativeWidget->show();
    ui->show();

    [macNSWindow setHasShadow:NO];
    [macNSWindow setLevel:kCGDesktopIconWindowLevel + 1];
    [macNSWindow makeKeyAndOrderFront:macNSWindow];
    [pool release];
    return qtApp.exec();
}
