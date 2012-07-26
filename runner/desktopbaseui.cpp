/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  :
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/

#include <QDebug>
#include <QDesktopWidget>

#include <pluginloader.h>
#include <abstractdesktopview.h>
#include <desktopviewplugin.h>

#include <config.h>
#include <plexyconfig.h>

#include "desktopbaseui.h"

#if defined(Q_WS_X11) // && defined(Q_WS_MAC) ??
#include <X11/Xlib.h>
#include <QX11Info>
#include <netwm.h>
#endif

using namespace PlexyDesk;

class DesktopBaseUi::DesktopBaseUiPrivate
{
    public:
        DesktopBaseUiPrivate () {}
        ~DesktopBaseUiPrivate () {}
        DesktopViewPlugin *mViewPlugin;
        AbstractDesktopView *mDesktopView;
        QDesktopWidget *mDesktopWidget;
        QGraphicsScene *mScene;
        PlexyDesk::Config *mConfig;
        QMap<int, AbstractDesktopView*> mViewList;
};


DesktopBaseUi::DesktopBaseUi(QWidget *parent) :
    QWidget(parent),
    d (new DesktopBaseUiPrivate)
{
    setDesktopView(QLatin1String ("plexydesktopview"));
    setup();

    connect (d->mDesktopWidget, SIGNAL(resized(int)), this, SLOT(screenResized(int)));
}


DesktopBaseUi::~DesktopBaseUi()
{
    Q_FOREACH (AbstractDesktopView * view, d->mViewList.values()) {
        if(view)
            delete view;
    }

    d->mViewList.clear();
}

void DesktopBaseUi::setDesktopView(const QString &name)
{
    d->mViewPlugin =
        static_cast<DesktopViewPlugin *>(
                PluginLoader::getInstance()->instance(name));
    //FIX : handle changes
}

void DesktopBaseUi::setup()
{
    d->mDesktopWidget = new QDesktopWidget ();
    d->mConfig = PlexyDesk::Config::getInstance();
    QSize desktopSize = (QSize) desktopRect().size();

    QGraphicsScene *scene = new QGraphicsScene;
    d->mScene = scene;

    for (int i = 0 ; i < d->mDesktopWidget->screenCount() ; i++ ) {
        desktopSize = d->mDesktopWidget->screenGeometry(i).size();
#ifdef Q_WS_WIN
        // A 1px hack to make the widget fullscreen and not covering the toolbar on Win
        desktopSize.setHeight(desktopSize.height()-1);
#endif
        QRect desktopScreenRect = d->mDesktopWidget->screenGeometry(i);
#ifdef Q_WS_WIN
        // A 1px hack to make the widget fullscreen and not covering the toolbar on Win
        desktopScreenRect.setHeight(desktopScreenRect.height()-1);
#endif
        //scene->setBackgroundBrush(Qt::NoBrush);
        scene->setItemIndexMethod(QGraphicsScene::NoIndex);

        scene->setBackgroundBrush(Qt::transparent);
        scene->setSceneRect(desktopScreenRect);

        if (!d->mViewPlugin) {
            continue;
        }

        AbstractDesktopView *view = d->mViewPlugin->view(scene);
        if (!view) {
            continue;
        }
#ifdef PLEXYNAME
        view->setWindowTitle(QString(PLEXYNAME));
#endif
        view->enableOpenGL(d->mConfig->isOpenGL());
        view->resize(desktopSize);
#ifdef PLEXYNAME
        view->setWindowTitle(QString(PLEXYNAME));
#endif

        view->enableOpenGL(d->mConfig->isOpenGL());
        view->move(d->mDesktopWidget->screenGeometry(i).x(),
                  d->mDesktopWidget->screenGeometry(i).y());
        view->setSceneRect (desktopScreenRect);
        view->ensureVisible(desktopScreenRect);
        view->setDragMode(QGraphicsView::RubberBandDrag);

#ifdef Q_WS_X11
        NETWinInfo info(QX11Info::display(), view->winId(), QX11Info::appRootWindow(), NET::WMDesktop );
        info.setDesktop(NETWinInfo::OnAllDesktops);
        info.setWindowType(NET::Desktop);
#endif
        view->showLayer(QLatin1String("Widgets"));
        d->mViewList[i] = view;
        QWidget *parentWidget = qobject_cast<QWidget*>(parent());
        if(parentWidget) {
            this->resize(view->size());
            view->setParent(this);

#ifdef Q_WS_MAC
            //TODO: until we write our own NSView we do this for mac (issue : 169)
            /*- (void)drawRect:(NSRect)rect {
                [[NSColor clearColor] set];
                NSRectFill(rect);
              }
           */
            view->setStyleSheet("background-color: transparent;");
#endif
        }
        view->show();
        QApplication::desktop()->setParent(view);
    }
}


void DesktopBaseUi::screenResized(int screen)
{
    if(!screen)
        return;

    AbstractDesktopView *view = d->mViewList[screen];
    QRect desktopScreenRect = d->mDesktopWidget->screenGeometry(screen);
#ifdef Q_WS_WIN
    // A 1px hack to make the widget fullscreen and not covering the toolbar on Win
    desktopScreenRect.setHeight(desktopScreenRect.height()-1);
#endif

    view->resize(desktopScreenRect.size());
}

QRect DesktopBaseUi::desktopRect() const
{
   if (d->mDesktopWidget->screenCount() == 1) {
        return d->mDesktopWidget->screenGeometry();
   }

   int total_width = 0;
   int total_height = 0;

   for (int i = 0 ; i < d->mDesktopWidget->screenCount(); i++) {
        total_width += d->mDesktopWidget->screenGeometry(i).width();
        total_height += d->mDesktopWidget->screenGeometry(i).height();
   }

   return QRect (0, 0, total_width, total_height);
}
