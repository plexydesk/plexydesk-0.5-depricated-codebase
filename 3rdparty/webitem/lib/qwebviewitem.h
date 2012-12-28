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

#ifndef QWEBVIEWITEM_H
#define QWEBVIEWITEM_H

#include <qwebkitglobal.h>
#include <qwebpage.h>
#include <QtWidgets/QWidget>
#include <QGraphicsRectItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QtCore/qurl.h>
#include <QtNetwork/qnetworkaccessmanager.h>

#include "config.h"

class QNetworkRequest;
class QPrinter;
class QWebPage;
class QWebViewItemPrivate;
class QWebNetworkRequest;

class WEBQGV_EXPORT QWebViewItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(QIcon icon READ icon)
    Q_PROPERTY(QString selectedText READ selectedText)
    Q_PROPERTY(bool modified READ isModified)
    Q_PROPERTY(qreal textSizeMultiplier READ textSizeMultiplier WRITE setTextSizeMultiplier)
public :
    explicit QWebViewItem(const QRectF &rect, QGraphicsItem *parent = 0);
    virtual ~QWebViewItem();

    QWebPage *page() const;
    void setPage(QWebPage *page);

    void load(const QUrl &url);

    void load(const QNetworkRequest &request,
    QNetworkAccessManager::Operation operation = QNetworkAccessManager::GetOperation,
    const QByteArray &body = QByteArray());
    void setHtml(const QString &html, const QUrl &baseUrl = QUrl());
    void setContent(const QByteArray &data, const QString &mimeType = QString(), const QUrl &baseUrl = QUrl());

    QWebHistory *history() const;
    QWebSettings *settings() const;

    QString title() const;
    void setUrl(const QUrl &url);
    QUrl url() const;
    QIcon icon() const;

    QString selectedText() const;

    QAction *pageAction(QWebPage::WebAction action) const;
    void triggerPageAction(QWebPage::WebAction action, bool checked = false);
    virtual QRectF boundingRect() const;
    bool isModified() const;
    QVariant inputMethodQuery(Qt::InputMethodQuery property) const;
    QSize sizeHint() const;
    void setTextSizeMultiplier(qreal factor);
    qreal textSizeMultiplier() const;
    bool findText(const QString &subString, QWebPage::FindFlags options = 0);
    QPixmap getPreview(bool fullpage = false);

public Q_SLOTS:
    void stop();
    void back();
    void forward();
    void reload();
    void loaded(bool);
    void repaintDirty(const QRect &);
    void scrollReqest(int, int, const QRect &);
    void print(QPrinter *printer) const;
    void changeOpacity(int);
    void startTimers();
    void fade();

Q_SIGNALS:
    void loadStarted();
    void loadProgress(int progress);
    void loadFinished(bool);
    void titleChanged(const QString &title);
    void statusBarMessage(const QString &text);
    void linkClicked(const QUrl &url);
    void selectionChanged();
    void iconChanged();
    void urlChanged(const QUrl &url);
    void previewUpdate();

protected:
    void resizeEvent(QResizeEvent *e);
    void paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

    virtual QWebViewItem *createWindow(QWebPage::WebWindowType type);
    virtual void changeEvent(QEvent *);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *);
    virtual void wheelEvent(QGraphicsSceneWheelEvent *);
    virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);
    virtual void dragEnterEvent(QDragEnterEvent *);
    virtual void dragLeaveEvent(QDragLeaveEvent *);
    virtual void dragMoveEvent(QDragMoveEvent *);
    virtual void dropEvent(QDropEvent *);
    virtual void focusInEvent(QFocusEvent *);
    virtual void focusOutEvent(QFocusEvent *);
    virtual void inputMethodEvent(QInputMethodEvent *);
    virtual bool focusNextPrevChild(bool next);
private:
    friend class QWebPage;
    QWebViewItemPrivate *d;
};

#endif // QWEBVIEW_H
