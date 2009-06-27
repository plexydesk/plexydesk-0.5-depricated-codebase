/*
 * Copyright (C) 2006 George Staikos <staikos@kde.org>
 * Copyright (C) 2006 Dirk Mueller <mueller@kde.org>
 * Copyright (C) 2006 Zack Rusin <zack@kde.org>
 * Copyright (C) 2006 Simon Hausmann <hausmann@kde.org>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <qwebpage.h>
#include <qwebview.h>
#include <qwebviewitem.h>
#include <qwebframe.h>
#include <desktopview.h>
#include <qwebsettings.h>
#include <QPainter>
#include <QtGui>
#include <QDebug>
#if QT_VERSION >= 0x040400
#include <QPrintPreviewDialog>
#endif

#include <QtUiTools/QUiLoader>


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QString url = QString("http://www.google.com");

    QWebSettings::setMaximumPagesInCache(4);
    QWebSettings::setObjectCacheCapacities((16*1024*1024)/8, (16*1024*1024)/8, 16*1024*1024);

    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

    const QStringList args = app.arguments();
    if (args.count() > 1)
        url = args.at(1);

    QUrl qurl(url);
    if (qurl.scheme().isEmpty())
        qurl = QUrl::fromLocalFile(QFileInfo(url).absoluteFilePath());
        
     QGraphicsScene * s = new  QGraphicsScene();
     WebView  window(s); ///WebView is a GraphicsView dose not relate to QWebView
     window.resize(800,480);
     window.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
     
     QWebViewItem * item = new QWebViewItem();
     item->setUrl(qurl);
     s->addItem(item);
     //item->hide();
     item->load(qurl);
     item->grabMouse();
     item->grabKeyboard();
     window.addWebPage(item);//some thing i needed for reflectiondemo 
//     item->setPos((window.width()- item->boundingRect().width())/2, (window.height() - item->boundingRect().height())/2);
      int x = 1 ;
      int y = x;
//     item->setTransform(QTransform().translate(x, y).scale(2.0,2.0).translate(-x, -y));
    window.show();

    return app.exec();
}
