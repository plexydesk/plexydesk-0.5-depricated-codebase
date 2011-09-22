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

#include <QGLWidget>
#include <QGraphicsGridLayout>
#include <QGraphicsDropShadowEffect>
#include <QFutureWatcher>
#include <QSharedPointer>
#include <QDir>
#include <QtDebug>

#include "desktopview.h"
#include "icon.h"
#include "iconprovider.h"
#include "themepackloader.h"
#include <desktopwidget.h>
#include <backdropinterface.h>
#include <pluginloader.h>
#include <baseplugin.h>
#include <backdropplugin.h>
#include <widgetplugin.h>
#include <viewlayer.h>
#include <qplexymime.h>

#if QT_VERSION < 0x04600
#include <QPropertyAnimation>
#endif

#ifdef Q_WS_X11
#include <plexywindow.h>

extern "C" {
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/shape.h>
#include <X11/cursorfont.h>
#include <GL/gl.h>  // Header File For The OpenGL32 Library
#include <GL/glu.h> // Header File For The GLu32 Library
}

#include <QX11Info>

#endif

#ifdef Q_WS_WIN
// Keep that if we want other native direct manipulations on Windows
//#include <windows.h>
#endif


using namespace PlexyDesk;

class DesktopView::Private
{
public:
    Private() {
    }
    ~Private()
    {
    }
    AbstractPluginInterface *bIface;
    BackdropPlugin *bgPlugin;
    PlexyDesk::ThemepackLoader *mThemeLoader;

    WidgetPlugin *mPhotoDialogProvider;
    DesktopWidget *mPhotoDialog;

    WidgetPlugin *mDirProvider;
    DesktopWidget *mDirWidget;

    float row;
    float column;
    float margin;
    bool openglOn;
    QGraphicsItem *backgroundWallpaperItem;
};

bool getLessThanWidget(const QGraphicsItem *it1, const QGraphicsItem *it2)
{
    return it1->zValue() < it2->zValue();
}

DesktopView::DesktopView(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent), d(new Private)
{
    /* Setup */

#ifdef Q_WS_WIN
    // The QT::Tool flag is needed so PlexyDesk is excluded from Win taskbar
    setWindowFlags(Qt::Tool |
                   Qt::FramelessWindowHint |
                   Qt::WindowStaysOnBottomHint);
#else
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::WindowStaysOnBottomHint);
#endif

    setFrameStyle(QFrame::NoFrame);

    setAttribute(Qt::WA_QuitOnClose);

#ifdef Q_WS_WIN
    // Needed so it gets no focus on win when starting up
    setAttribute(Qt::WA_ShowWithoutActivating);

    // Keep the next one if we ever need native direct manipulation on Win
    //::SetWindowPos(this->winId(), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );
#endif

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setAlignment(Qt::AlignLeft | Qt::AlignTop);


    d->openglOn = false;

    /* Init */

    d->mThemeLoader = new PlexyDesk::ThemepackLoader(PlexyDesk::Config::getInstance()->themepackName(), this);
    d->mPhotoDialogProvider = NULL;
    d->mPhotoDialog = NULL;
    d->mDirProvider = NULL;
    d->mDirWidget = NULL;

    d->mThemeLoader = new ThemepackLoader(PlexyDesk::Config::getInstance()->themepackName(), this);
    qDebug() << Q_FUNC_INFO << PlexyDesk::Config::getInstance()->wallpaper();
    if (PlexyDesk::Config::getInstance()->wallpaper().isEmpty()) {
        qDebug() << Q_FUNC_INFO << d->mThemeLoader->wallpaper();
        PlexyDesk::Config::getInstance()->setWallpaper(d->mThemeLoader->wallpaper());
    }
    d->bgPlugin = static_cast<BackdropPlugin *>(PluginLoader::getInstance()->instance("classicbackdrop"));
    d->row = d->column = 48.0;
    d->margin = 10.0;

    connect(Config::getInstance(), SIGNAL(configChanged()), this, SLOT(backgroundChanged()));
    connect(Config::getInstance(), SIGNAL(widgetAdded()), this, SLOT(onNewWidget()));

#ifdef Q_WS_X11
    if (checkXCompositeExt()) {
        qDebug() << Q_FUNC_INFO << "Supports Composite Ext: Yes";
        // TODO: load composite layer
    } else {
        qDebug() << Q_FUNC_INFO << "Supports Composite Ext: No";
    }
#endif
}

void DesktopView::onNewWidget()
{
   // addExtension(Config::getInstance()->widgetList.last());
}

void DesktopView::closeDesktopWidget()
{
    qDebug() << Q_FUNC_INFO;
    DesktopWidget *widget = qobject_cast<DesktopWidget *> (sender());
    if (widget) {
        scene()->removeItem(widget);
        delete widget;
    }
}
void DesktopView::enableOpenGL(bool state)
{
    if (state) {
        setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers |
                        QGL::StencilBuffer |
                        QGL::DoubleBuffer |
                        QGL::AlphaChannel)));
        setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing);
        d->openglOn = true;
    } else {
        setViewport(new QWidget);
        setCacheMode(QGraphicsView::CacheBackground);
        setOptimizationFlags(QGraphicsView::DontSavePainterState);
        setOptimizationFlag(QGraphicsView::DontClipPainter);
        setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        d->openglOn = false;
    }
}

void DesktopView::showLayer(const QString &layer)
{
    Q_UNUSED(layer);
}

void DesktopView::setThemePack(const QString &name)
{
    if (d->mThemeLoader) {
        delete d->mThemeLoader;

        // TODO: ERROR HANDLING
        d->mThemeLoader = new PlexyDesk::ThemepackLoader(name, this);
        d->mThemeLoader->setRect (scene()->sceneRect());
        if (PlexyDesk::Config::getInstance()->wallpaper().isEmpty()) {
            PlexyDesk::Config::getInstance()->setWallpaper(d->mThemeLoader->wallpaper());
        }

        Q_FOREACH(const QString &nativeWidget, d->mThemeLoader->widgets("native")) {
            qDebug() << Q_FUNC_INFO << nativeWidget;
            QPoint pos = d->mThemeLoader->widgetPos(nativeWidget);
            addExtension(nativeWidget, QLatin1String("Widgets"), pos,
                    d->mThemeLoader->widgetView(nativeWidget));
        }

        Q_FOREACH(const QString &qmlWidget, d->mThemeLoader->widgets("QML")) {
            qDebug() << Q_FUNC_INFO << "Loading QML: " << qmlWidget;
            // FIXME : Memory leak
            DesktopWidget *parent = new DesktopWidget(QRectF(0,0,0,0), 0, 0);
            parent->qmlFromUrl(QUrl(d->mThemeLoader->qmlFilesFromTheme(qmlWidget)));
            scene()->addItem(parent);
            connect(parent, SIGNAL(close()), this, SLOT(closeDesktopWidget()));
            QPoint pos = d->mThemeLoader->widgetPos(qmlWidget);
            parent->setPos(pos);

            //check shadow prop

            bool shadow_on = d->mThemeLoader->getProperty(qmlWidget, "shadow").toBool();
            qDebug() << Q_FUNC_INFO << qmlWidget << " Shadow On :" << shadow_on;
            if(shadow_on) {
              QGraphicsDropShadowEffect * effect  = new QGraphicsDropShadowEffect(this);
              effect->setBlurRadius(8.0);
              effect->setColor(QColor(0, 0, 0));
              parent->setGraphicsEffect(effect);
            }
        }

    }
}

void DesktopView::addWallpaperItem()
{
    if (d->bgPlugin) {
        QRectF screenRect = scene()->sceneRect();
        d->bgPlugin->setRect (
            QRect (screenRect.x(),
              screenRect.y(),
              screenRect.width(),
              screenRect.height()));

        d->backgroundWallpaperItem = d->bgPlugin->item();
        if (d->backgroundWallpaperItem) {
            scene()->addItem(d->backgroundWallpaperItem);
            d->backgroundWallpaperItem->show();
            d->backgroundWallpaperItem->setPos(screenRect.x(), screenRect.y());
            scene()->update();
        }
    }
}

#ifdef Q_WS_X11
bool DesktopView::checkXCompositeExt()
{
    /* We don't support this feature on Windows and MacosX */

    int event_base, error_base;
    if (XCompositeQueryExtension(QX11Info::display(), &event_base, &error_base)) {
        int major = 0, minor = 2;
        XCompositeQueryVersion(QX11Info::display(), &major, &minor );
        if ( major > 0 || minor >= 2 ) {
            return true;
        }
    }
    return false;
}

void DesktopView::redirectWindows()
{
    qDebug() << Q_FUNC_INFO;

    Display *dpy = QX11Info::display();

    for (int i = 0; i < ScreenCount(dpy); i++) {
        XCompositeRedirectSubwindows(dpy, RootWindow(dpy, i),
                                CompositeRedirectAutomatic);
    }
}

void DesktopView::loadWindows()
{

    Display *dpy = QX11Info::display();
    XGrabServer (dpy);
    Window root_notused, parent_notused;
    Window *children;
    unsigned int nchildren;
    XWindowAttributes attr;

    XQueryTree (dpy,
     RootWindow(dpy, QX11Info::appScreen()),
     &root_notused,
     &parent_notused,
     &children,
     &nchildren);

    for (int i = 0; i < nchildren; i++) {
        if (not XGetWindowAttributes(QX11Info::display(), children[i], &attr)) {
            qDebug() << Q_FUNC_INFO << "Failed to get window attribute";
            continue;
        }

        if (children[i] == this->winId()) {
            continue;
        }
        if (attr.map_state == IsViewable) {
           qDebug() << Q_FUNC_INFO << "Windows found";
           //XRenderPictFormat *format = XRenderFindVisualFormat(dpy, attr.visual);
           //XRenderPictureAttributes pa;
           //pa.subwindow_mode = IncludeInferiors; // Don't clip child widgets
           //Picture picture = XRenderCreatePicture( dpy, children[i], format, CPSubwindowMode, &pa );
           PlexyWindows *_window = new PlexyWindows(dpy, children[i], &attr);
           _window->configState(DesktopWidget::NORMALSIDE);
           scene()->addItem(_window);
           _window->setRect(QRectF(0,0, 200, 200));
           _window->show();
        }
    }
    XUngrabServer (dpy);
}
#endif

DesktopView::~DesktopView()
{
    delete d;
}

void DesktopView::backgroundChanged()
{
    if (d->bgPlugin) {
        delete d->bgPlugin;
    }
    d->bgPlugin =
         static_cast<BackdropPlugin *>(PluginLoader::getInstance()->instance("classicbackdrop"));
    if (!d->openglOn) {
        setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    }
    setCacheMode(QGraphicsView::CacheNone);
    invalidateScene();
    scene()->update();
    update();

    if (!d->openglOn) {
        setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    }

    setCacheMode(QGraphicsView::CacheBackground);
}

/*
   \brief Adds an Widget Extension to Plexy Desktop, give the widget
   name in string i.e "clock" or "radio", the internals will
   take care of the loading the widget plugin name is correct

   \param name String name of the widget as specified by the desktop file

   \param layerName Name of the layer you want add the widget to
 */

void DesktopView::addExtension(const QString &name,
        const QString &layerName,
        const QPoint &pos,
        PlexyDesk::DesktopWidget::State state)
{
    WidgetPlugin *provider = static_cast<WidgetPlugin *>(PluginLoader::getInstance()->instance(name));
    if (provider) {
        qDebug() << Q_FUNC_INFO << name << layerName;
        DesktopWidget *widget = (DesktopWidget *) provider->item();
        if (widget) {
            widget->configState(state);
            scene()->addItem(widget);
            if (pos.x() == 0 && pos.y() == 0) {
                widget->setPos(d->row, d->column);
                //d->row += widget->boundingRect().width()+d->margin;
            } else {
                widget->setPos(pos);
            }

            connect(widget, SIGNAL(close()), this, SLOT(closeDesktopWidget()));
        }
    }
   delete provider;
}

void DesktopView::addCoreExtension(const QString &name)
{
    WidgetPlugin *provider = static_cast<WidgetPlugin *>(PluginLoader::getInstance()->instance(name));
    if (provider) {
        QGraphicsRectItem *widget = (QGraphicsRectItem *) provider->item();
        if (widget) {
            scene()->addItem(widget);
            widget->setPos(d->row, d->column);
            d->row += widget->boundingRect().width();
        }
    }
    delete provider;
}

// Small speed up , try if the speed is too low
/*
void DesktopView::paintEvent(QPaintEvent * event)
{
   QPaintEvent *newEvent=new QPaintEvent(event->region().boundingRect());
   QGraphicsView::paintEvent(newEvent);
   delete newEvent;
}

*/

void DesktopView::dropEvent(QDropEvent * event)
{
    if ( !event )
        return;

    if ( event->mimeData()->urls().count() <= 0 ) {
        qDebug() << "Drop ignored...";
        return;
    }

    // FIXME: Refactor this ugly handling
    const QString droppedFile = event->mimeData()->urls().value(0).toLocalFile();

    int droppedType = checkDropped(droppedFile);

    if (droppedType == folder) {
        /* handle folders */
        qDebug() << Q_FUNC_INFO << "Dir Drop accepted...";

        WidgetPlugin *dirWidgetPlugin = registerHandler (QLatin1String("folderwidget"));

        if(dirWidgetPlugin) {
            QVariantMap data;
            data["dir_path"] = QVariant("/" + droppedFile);
            dirWidgetPlugin->setData(data);
        } else {
            qWarning() << Q_FUNC_INFO << "Dir Handler Not registered";
        }
    } else if (droppedType == qml) {
        qDebug() << Q_FUNC_INFO << "QML Drop accepted...";
        DesktopWidget *parent = new DesktopWidget(QRectF(0,0,0,0), 0, 0);
        parent->qmlFromUrl(QUrl(droppedFile));
        scene()->addItem(parent);
        connect(parent, SIGNAL(close()), this, SLOT(closeDesktopWidget()));
        event->acceptProposedAction();
    } else if (droppedType == img) {
        qDebug() << Q_FUNC_INFO << "Image Drop accepted...";
        if (d->mPhotoDialog) {
            d->mPhotoDialog->show();
            QVariantMap data;
            data["photo_path"] = QVariant(droppedFile);

            if (d->mPhotoDialogProvider) {
                d->mPhotoDialogProvider->setData(data);
            }
        }
    } else {
        qDebug() << Q_FUNC_INFO << "Drop ignored...";
        return;
    }

    event->acceptProposedAction();
}

void DesktopView::dragEnterEvent (QDragEnterEvent * event)
{
    if ( !event )
        return;

    if ( event->mimeData()->urls().count() >= 0 ) {
        const QString droppedFile = event->mimeData()->urls().value(0).toLocalFile();
        if ( checkDropped(droppedFile) != none )
            event->accept();
    }
}

int DesktopView::checkDropped (const QString &file)
{
    if ( file.contains(".qml") )
        return qml;

    QFileInfo info(file);
    if ( info.isDir() )
        return folder;

    if ( !info.isDir() && !QPixmap(file).isNull() )
        return img;

    return none;
}

void DesktopView::dragMoveEvent (QDragMoveEvent * event)
{
    if ( !event )
        return;

    event->accept();
}

void DesktopView::registerPhotoDialog()
{
    if (d->mPhotoDialog) {
        delete d->mPhotoDialog;
    }

    if (d->mPhotoDialogProvider) {
        delete d->mPhotoDialogProvider;
    }

    const QString name = "photoframe";
    const QPoint pos (0.0, 0.0);
    WidgetPlugin *provider = static_cast<WidgetPlugin *>(PluginLoader::getInstance()->instance(name));
    if (provider) {
        DesktopWidget *widget = (DesktopWidget *) provider->item();
        if (widget) {
            //widget->configState(state);
            scene()->addItem(widget);

            const QRectF viewRect (0.0, 0.0, this->width(), this->height());
            const QPointF center = viewRect.center();

            float x_pos = center.x() - (widget->boundingRect().width()/2);
            float y_pos = center.y() - (widget->boundingRect().height()/2);

            widget->setPos(x_pos, y_pos);

            d->mPhotoDialog = widget;
            connect(widget, SIGNAL(close()), this, SLOT(closeDesktopWidget()));
            d->mPhotoDialog->hide();
            d->mPhotoDialog->configState(DesktopWidget::NORMALSIDE);
        }
    }

    d->mPhotoDialogProvider = provider;
}

//FIXME: Refactor so that we can avoid having a method per type
WidgetPlugin *DesktopView::registerHandler(const QString &name, bool effects_on)
{
    const QPoint pos (0.0, 0.0);
    WidgetPlugin *provider = static_cast<WidgetPlugin *>(PluginLoader::getInstance()->instance(name));
    if (provider) {
        DesktopWidget *widget = (DesktopWidget *) provider->item();
        if (widget) {
            scene()->addItem(widget);

            const QRectF viewRect (0.0, 0.0, this->width(), this->height());
            const QPointF center = viewRect.center();

            float x_pos = center.x() - (widget->boundingRect().width()/2);
            float y_pos = center.y() - (widget->boundingRect().height()/2);

            widget->setPos(x_pos, y_pos);

            widget->configState(DesktopWidget::NORMALSIDE);

            if (effects_on) {
              QGraphicsDropShadowEffect * effect  = new QGraphicsDropShadowEffect(this);
              effect->setBlurRadius(8.0);
              effect->setColor(QColor(0, 0, 0));

              widget->setGraphicsEffect(effect);
            }
        }
        return provider;
    }

    return NULL;
}

/*

void DesktopView::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setRenderHint(QPainter::TextAntialiasing, false);
#ifdef Q_WS_X11
    painter->setRenderHint(QPainter::QPainter::SmoothPixmapTransform, false);
#endif
    painter->setRenderHint(QPainter::HighQualityAntialiasing, false);
    painter->setCompositionMode(QPainter::CompositionMode_Source);
    painter->setClipRect(rect);

    painter->save();
    painter->restore();
}

*/

void DesktopView::mousePressEvent(QMouseEvent *event)
{
    // Commented out due to QML stacking problem
    //setTopMostWidget(event->pos());
    QGraphicsView::mousePressEvent(event);
}

void DesktopView::setTopMostWidget(const QPoint &pt)
{
    int i = 0;
    QGraphicsItem *clickedItem = scene()->itemAt(pt);
    if (clickedItem == 0)
        return;

    QList<QGraphicsItem *> itemsList = scene()->items();
    qStableSort(itemsList.begin(), itemsList.end(), getLessThanWidget);

    clickedItem->setZValue(itemsList.size());

    foreach(QGraphicsItem* item, itemsList) {
        if (item == clickedItem)
            continue;

        item->setZValue(i);
        i++;
    }

    clickedItem->update();
}

void DesktopView::keyReleaseEvent(QKeyEvent *event)
{
    if((event->key() == Qt::Key_Q) && event->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier))
        emit closeApplication();

    QGraphicsView::keyReleaseEvent(event);
}
