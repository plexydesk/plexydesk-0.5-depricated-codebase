/*
 * qwebviewitem.h A Web content render for QGraphicsView
 *
 * Stefani Project
 * Copyright (C) 2007 Trolltech ASA
 * Copyright (C) 2008 Holger Hans Peter Freyther
 * Copyright (C) 2008 Collabora Ltd.
 * Copyright (C) 2008 Nokia Corporation
 *   @author Siraj Razick <siraj.razick@collabora.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

//#include "config.h"
#include <qwebviewitem.h>
#include <qwebframe.h>
//#include <qwebpage_p.h>

#include <qbitmap.h>
#include <qevent.h>
#include <qpainter.h>
#include <qprinter.h>
#include <qgraphicsview.h>
#include <QEvent>
#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QTimeLine>
#include <QtCore>
class QWebViewItemPrivate
{
public:
    QWebViewItemPrivate(QWebViewItem *view)
        : view(view)
          , page(0)
#ifndef QT_NO_CURSOR
          , cursorSetByWebCore(false)
          , usesWebCoreCursor(false)
#endif
    {
    }

    QWebViewItem *view;
    QWebPage *page;
    QRect viewRect;
    float opacity;
    QTimeLine *fadeline;
    QPixmap previewCache;


#ifndef QT_NO_CURSOR
    /*
     * We keep track of if we have called setCursor and if the CursorChange
     * event is sent due our setCursor call and if we currently use the WebCore
     * Cursor and use it to decide if we can update to another WebCore Cursor.
     */
    bool cursorSetByWebCore;
    bool usesWebCoreCursor;

    void setCursor(const QCursor &newCursor)
    {
        webCoreCursor = newCursor;

        if (usesWebCoreCursor) {
            cursorSetByWebCore = true;
            view->setCursor(webCoreCursor);
        }
    }

    QCursor webCoreCursor;
#endif
};

QWebViewItem::QWebViewItem(const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsRectItem(rect, parent)
{
    qDebug() << Q_FUNC_INFO << rect;
    d = new QWebViewItemPrivate(this);
    d->opacity = 1.0;
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setAcceptsHoverEvents (true);

    // FIXME:should take a rect as argument
    d->previewCache = QPixmap(rect.width(), rect.height());
    setRect(rect);
}

void QWebViewItem::changeOpacity(int frm)
{
    d->opacity = frm/100.0;
    update();
}

void QWebViewItem::startTimers()
{
}

QPixmap QWebViewItem::getPreview(bool fullpage)
{
    if (fullpage) {
        //TODO
    } else {
        QPainter p(&d->previewCache);
        p.fillRect(boundingRect(), Qt::transparent);
        scene()->render(&p, boundingRect());
    }

    return d->previewCache;
}

/*!
    Destroys the web view.
 */
QWebViewItem::~QWebViewItem()
{
    if (d->page && d->page->parent() == this)
        delete d->page;
    delete d;
}

/*!
    Returns a pointer to the underlying web page.

    \sa setPage()
 */
QWebPage *QWebViewItem::page() const
{
    if (!d->page) {
        QWebViewItem *that = const_cast<QWebViewItem *>(this);
        that->setPage(new QWebPage(that));
    }
    return d->page;
}

/*!
    Makes \a page the new web page of the web view.

    The parent QObject of the provided page remains the owner
    of the object. If the current document is a child of the web
    view, it will be deleted.

    \sa page()
 */
void QWebViewItem::setPage(QWebPage *page)
{
    if (d->page == page)
        return;
    if (d->page) {
        if (d->page->parent() == this) {
            delete d->page;
        } else {
            d->page->disconnect(this);
        }
    }
    d->page = page;
    if (d->page) {
        QWebFrame *mainFrame = d->page->mainFrame();
        mainFrame->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
        mainFrame->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
        connect(mainFrame, SIGNAL(titleChanged(const QString &)),
         this, SIGNAL(titleChanged(const QString &)));
        connect(mainFrame, SIGNAL(iconChanged()),
         this, SIGNAL(iconChanged()));
        connect(mainFrame, SIGNAL(urlChanged(const QUrl &)),
         this, SIGNAL(urlChanged(const QUrl &)));

        connect(d->page, SIGNAL(loadStarted()),
         this, SIGNAL(loadStarted()));

        connect(d->page, SIGNAL(loadProgress(int)),
         this, SIGNAL(loadProgress(int)));
        connect(d->page, SIGNAL(loadFinished(bool)),
         this, SIGNAL(loadFinished(bool)));
        connect(d->page, SIGNAL(statusBarMessage(const QString &)),
         this, SIGNAL(statusBarMessage(const QString &)));
        connect(d->page, SIGNAL(linkClicked(const QUrl &)),
         this, SIGNAL(linkClicked(const QUrl &)));

        connect(d->page, SIGNAL(repaintRequested(const QRect &)), this, SLOT(repaintDirty(const QRect &)));
        connect(d->page, SIGNAL(scrollRequested(int, int, const QRect &)), this, SLOT(scrollReqest(int, int, const QRect &)));
        connect(this, SIGNAL(loadFinished(bool)), this, SLOT(loaded(bool)));
        connect(this, SIGNAL(loadStarted()), this, SLOT(startTimers()));
    }

}

void QWebViewItem::scrollReqest(int dx, int dy, const QRect &r)
{
    update(r);
}

void QWebViewItem::repaintDirty(const QRect &rect)
{

    update(rect);
}


void QWebViewItem::load(const QUrl &url)
{
    page()->mainFrame()->load(url);
}



#if QT_VERSION < 0x040400 && !defined(qdoc)
void QWebViewItem::load(const QWebNetworkRequest &request)
#else
void QWebViewItem::load(const QNetworkRequest &request,
 QNetworkAccessManager::Operation operation,
 const QByteArray &body)
#endif
{
    page()->mainFrame()->load(request
#if QT_VERSION >= 0x040800
     , operation, body
#endif
     );
}


void QWebViewItem::setHtml(const QString &html, const QUrl &baseUrl)
{
    page()->mainFrame()->setHtml(html, baseUrl);
}


void QWebViewItem::setContent(const QByteArray &data, const QString &mimeType, const QUrl &baseUrl)
{
    page()->mainFrame()->setContent(data, mimeType, baseUrl);
}


QWebHistory *QWebViewItem::history() const
{
    return page()->history();
}


QWebSettings *QWebViewItem::settings() const
{
    return page()->settings();
}


QString QWebViewItem::title() const
{
    if (d->page)
        return d->page->mainFrame()->title();
    return QString();
}


void QWebViewItem::setUrl(const QUrl &url)
{
    page()->mainFrame()->setUrl(url);
}

QUrl QWebViewItem::url() const
{
    if (d->page)
        return d->page->mainFrame()->url();
    return QUrl();
}

/*!
    \property QWebViewItem::icon
    \brief the icon associated with the web page currently viewed

    \sa iconChanged(), QWebSettings::iconForUrl()
 */                                                                                                                                                           //
QIcon QWebViewItem::icon() const
{
    if (d->page)
        return d->page->mainFrame()->icon();
    return QIcon();
}

/*!
    \property QWebViewItem::selectedText
    \brief the text currently selected

    \sa findText(), selectionChanged()
 */
QString QWebViewItem::selectedText() const
{
    if (d->page)
        return d->page->selectedText();
    return QString();
}

/*!
    Returns a pointer to a QAction that encapsulates the specified web action \a action.
 */
QAction *QWebViewItem::pageAction(QWebPage::WebAction action) const
{
    return page()->action(action);
}

/*!
    Triggers the specified \a action. If it is a checkable action the specified
    \a checked state is assumed.

    The following example triggers the copy action and therefore copies any
    selected text to the clipboard.

    \snippet doc/src/snippets/code/src.3rdparty.webkit.WebKit.qt.Api.qwebview.cpp 2

    \sa pageAction()
 */
void QWebViewItem::triggerPageAction(QWebPage::WebAction action, bool checked)
{
    page()->triggerAction(action, checked);
}

/*!
    \property QWebViewItem::modified
    \brief whether the document was modified by the user

    Parts of HTML documents can be editable for example through the
    \c{contenteditable} attribute on HTML elements.
 */
bool QWebViewItem::isModified() const
{
    if (d->page)
        return d->page->isModified();
    return false;
}

/*
   Qt::TextInteractionFlags QWebViewItem::textInteractionFlags() const
   {
    // ### FIXME (add to page)
    return Qt::TextInteractionFlags();
   }
 */

/*
    \property QWebViewItem::textInteractionFlags
    \brief how the view should handle user input

    Specifies how the user can interact with the text on the page.
 */

/*
   void QWebViewItem::setTextInteractionFlags(Qt::TextInteractionFlags flags)
   {
    Q_UNUSED(flags)
    // ### FIXME (add to page)
   }
 */

/*!
    \reimp
 */
QSize QWebViewItem::sizeHint() const
{
    return QSize(rect().width(), rect().height());
}

QRectF QWebViewItem::boundingRect() const
{
    return rect();
}


/*!
   \property QWebViewItem::textSizeMultiplier
   \brief the scaling factor for all text in the frame
 */

void QWebViewItem::setTextSizeMultiplier(qreal factor)
{
    page()->mainFrame()->setTextSizeMultiplier(factor);
}

qreal QWebViewItem::textSizeMultiplier() const
{
    return page()->mainFrame()->textSizeMultiplier();
}

bool QWebViewItem::findText(const QString &subString, QWebPage::FindFlags options)
{
    if (d->page)
        return d->page->findText(subString, options);
    return false;
}

void QWebViewItem::print(QPrinter *printer) const
{
#ifndef QT_NO_PRINTER
    page()->mainFrame()->print(printer);
#endif
}

void QWebViewItem::stop()
{
    if (d->page)
        d->page->triggerAction(QWebPage::Stop);
}

void QWebViewItem::back()
{
    if (d->page)
        d->page->triggerAction(QWebPage::Back);
}


void QWebViewItem::fade()
{
    if (d->fadeline->direction() == QTimeLine::Backward)
        d->fadeline->setDirection(QTimeLine::Forward);
    d->fadeline->start();
}
void QWebViewItem::loaded(bool state)
{

    if (state) {

        emit previewUpdate();
    }
    d->page->setViewportSize(QSize(rect().width(), rect().height()));
}
void QWebViewItem::forward()
{
    if (d->page)
        d->page->triggerAction(QWebPage::Forward);
}

/*!
    Reloads the current document.

    \sa stop(), pageAction(), loadStarted()
 */
void QWebViewItem::reload()
{
    if (d->page)
        d->page->triggerAction(QWebPage::Reload);
}

/*! \reimp
 */
void QWebViewItem::resizeEvent(QResizeEvent *e)
{
    if (d->page)
        d->page->setViewportSize(e->size());
}

/*! \reimp
 */
void QWebViewItem::paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget  )
{
    if (!d->page)
        return;

    if (isObscured())
        return;

    if (!painter->isActive())
        return;

    painter->setClipping(true);
    painter->setOpacity(this->opacity());
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    QWebFrame *frame = d->page->mainFrame();
    frame->render(painter, QRegion(QRect(option->exposedRect.x(),
             option->exposedRect.y(),
             option->exposedRect.width(),
             option->exposedRect.height())));
    painter->setClipRect(rect());
}

/*!
    This function is called whenever WebKit wants to create a new window of the given \a type, for example as a result of
    a JavaScript request to open a document in a new window.

    \sa QWebPage::createWindow()
 */
QWebViewItem *QWebViewItem::createWindow(QWebPage::WebWindowType type)
{
    Q_UNUSED(type)
    return 0;
}

/*! \reimp
 */
void QWebViewItem::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
    //hack
    if (d->page) {
        QMouseEvent fakeMouseEvent(QEvent::MouseMove, ev->pos().toPoint(), ev->button(), ev->buttons(), ev->modifiers());
        d->page->event(&fakeMouseEvent);
        ev->accept();

        emit previewUpdate();
    } else {
        QGraphicsRectItem::mouseMoveEvent(ev);
    }

}

/*! \reimp
 */
void QWebViewItem::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    if (d->page) {
        QMouseEvent fakeMouseEvent(QEvent::MouseButtonPress, ev->pos().toPoint(), ev->button(), ev->buttons(), ev->modifiers());
        d->page->event(&fakeMouseEvent);
        d->page->event(ev);
        ev->accept();
        emit previewUpdate();
    }
}

/*! \reimp
 */
void QWebViewItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *ev)
{
    if (d->page) {
        QMouseEvent fakeMouseEvent(QEvent::MouseButtonDblClick, ev->pos().toPoint(), ev->button(), ev->buttons(), ev->modifiers());
        d->page->event(&fakeMouseEvent);
        d->page->event(ev);
        ev->accept();
    }
    QGraphicsRectItem::mouseDoubleClickEvent(ev);
}

/*! \reimp
 */
void QWebViewItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    if (d->page) {
        QMouseEvent fakeMouseEvent(QEvent::MouseButtonRelease, ev->pos().toPoint(), ev->button(), ev->buttons(), ev->modifiers());
        d->page->event(&fakeMouseEvent);
        d->page->event(ev);
        ev->accept();
        QGraphicsRectItem::mouseReleaseEvent(ev);
    }
}

/*! \reimp
 */
void QWebViewItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *ev)
{
    if (d->page)
        d->page->event(ev);

    QGraphicsRectItem::contextMenuEvent(ev);
}

/*! \reimp
 */
void QWebViewItem::wheelEvent(QGraphicsSceneWheelEvent *ev)
{
    if (d->page) {
        QWheelEvent fakeWheelEvent(ev->pos().toPoint(), ev->delta(), ev->buttons(), ev->modifiers(), ev->orientation());
        d->page->event(&fakeWheelEvent);
        ev->accept();
    }
}

/*! \reimp
 */
void QWebViewItem::keyPressEvent(QKeyEvent *ev)
{
    if (d->page)
        d->page->event(ev);
    if (!ev->isAccepted())
        QGraphicsRectItem::keyPressEvent(ev);
}

/*! \reimp
 */
void QWebViewItem::keyReleaseEvent(QKeyEvent *ev)
{
    if (d->page)
        d->page->event(ev);
    emit previewUpdate();
    if (!ev->isAccepted())
        QGraphicsRectItem::keyReleaseEvent(ev);
}

/*! \reimp
 */
void QWebViewItem::focusInEvent(QFocusEvent *ev)
{
    if (d->page)
        d->page->event(ev);
    QGraphicsRectItem::focusInEvent(ev);

}

/*! \reimp
 */
void QWebViewItem::focusOutEvent(QFocusEvent *ev)
{
    QGraphicsRectItem::focusOutEvent(ev);
    if (d->page)
        d->page->event(ev);
    emit previewUpdate();
}

/*! \reimp
 */
void QWebViewItem::dragEnterEvent(QDragEnterEvent *ev)
{
#ifndef QT_NO_DRAGANDDROP
    if (d->page)
        d->page->event(ev);
#endif
}

/*! \reimp
 */
void QWebViewItem::dragLeaveEvent(QDragLeaveEvent *ev)
{
#ifndef QT_NO_DRAGANDDROP
    if (d->page)
        d->page->event(ev);
#endif
}

/*! \reimp
 */
void QWebViewItem::dragMoveEvent(QDragMoveEvent *ev)
{
#ifndef QT_NO_DRAGANDDROP
    if (d->page)
        d->page->event(ev);
#endif
}

/*! \reimp
 */
void QWebViewItem::dropEvent(QDropEvent *ev)
{
#ifndef QT_NO_DRAGANDDROP
    if (d->page)
        d->page->event(ev);
#endif
}

/*! \reimp
 */
bool QWebViewItem::focusNextPrevChild(bool next)
{
    if (d->page && d->page->focusNextPrevChild(next))
        return true;
    return false;
}

/*!\reimp
 */
QVariant QWebViewItem::inputMethodQuery(Qt::InputMethodQuery property) const
{
    if (d->page)
        return d->page->inputMethodQuery(property);
    return QVariant();
}

/*!\reimp
 */
void QWebViewItem::inputMethodEvent(QInputMethodEvent *e)
{
    if (d->page)
        d->page->event(e);
}

/*!\reimp
 */
void QWebViewItem::changeEvent(QEvent *e)
{
    if (d->page && e->type() == QEvent::PaletteChange) {
        //d->page->setPalette(palette());
    }
}
