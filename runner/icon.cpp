/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
 */

#include "icon.h"
#include <iconprovider.h>

#include <QPainter>
#include <QProcess>

namespace PlexyDesk
{

class Icon::Private
{
public:
    Private() {
    }
    ~Private() {
    }
    QPixmap icon;
    QPlexyMime *mime;
    QString text;
    bool valid;
    IconProviderPtr iconprovider;
    uint id;
    IconJobPtr iconjob;
    QString exe;
    QString path;
};

Icon::Icon(IconProviderPtr icon, QPlexyMime *mime, const QRectF &rect, QWidget *embeddedWidget) : DesktopWidget(rect), d(new Private)
{
    d->valid = false;
    d->iconprovider = icon;
    d->id = 0;
    d->mime = mime;
    //connect(this, SIGNAL(pathSet()), this, SLOT(loadContent()));
}

Icon::~Icon()
{
    delete d;
}

void Icon::loadContent()
{
    d->mime->fromFileName(d->path);
}

void Icon::fromFileNameMime(const MimePairType mimePair)
{
    QString iconname;
    if (mimePair.second == "application/x-desktop") {
        QSettings setting(d->path, QSettings::IniFormat);
        setting.beginGroup("Desktop Entry");
        iconname = setting.value("Icon", "").toString();
        d->text = setting.value("Name", "").toString();
        d->exe = setting.value("Exec", "").toString();
        setting.endGroup();
        d->valid = true;
        d->iconjob = d->iconprovider->requestIcon(iconname, "32x32");
        connect(d->iconjob.data(), SIGNAL(finished()), this, SLOT(loadIcon()));
        emit iconLoaded();
        qDebug() << Q_FUNC_INFO << mimePair;
    } else {
        d->mime->genericIconName(mimePair.second);
    }
}

void Icon::genericIconNameMime(const MimePairType mimePair)
{
    qDebug() << Q_FUNC_INFO << mimePair;
    emit iconLoaded();
}

void Icon::loadIcon()
{
    d->icon = d->iconjob->Icon();
    update();
}

void Icon::setContent(const QString &path)
{
    d->path = path;
    //QMetaObject::invokeMethod(this, "pathSet", Qt::DirectConnection);
}

bool Icon::isValid()
{
    return d->valid;
}

void Icon::paintBackSide(QPainter *painter, const QRectF &rect)
{
    if (!d->icon.isNull()) {
        int x = (this->boundingRect().width() - d->icon.width()) / 2;
        painter->drawPixmap(QRect(x, x, d->icon.width(), d->icon.height()), d->icon);
    }
    drawText(painter, rect);
    // DesktopWidget::paintDockView(painter, rect);
}
void Icon::paintViewSide(QPainter *painter, const QRectF &rect)
{
    if (!d->icon.isNull()) {
        int x = (this->boundingRect().width() - d->icon.width()) / 2;
        painter->drawPixmap(QRect(x, x, d->icon.width(), d->icon.height()), d->icon);
    }
    drawText(painter, rect);
}
void Icon::paintDockView(QPainter *painter, const QRectF &rect)
{
    if (!d->icon.isNull()) {
        int x = (this->boundingRect().width() - d->icon.width()) / 2;
        painter->drawPixmap(QRect(x, x, d->icon.width(), d->icon.height()), d->icon);
    }
    drawText(painter, rect);
}

void Icon::drawText(QPainter *painter, const QRectF &rect)
{
    float x = (this->boundingRect().width() - d->icon.width()) / 2;
    float y = boundingRect().height() - x; // - (d->icon.height() +x );
    QTextOption opt;
    opt.setAlignment(Qt::AlignHCenter);
    QFontMetrics fm(painter->font());
    QString label = fm.elidedText(d->text, Qt::ElideRight, boundingRect().width());
    QRectF tr = QRectF(0.0, y, boundingRect().width(), boundingRect().height() -
             (d->icon.height()));
    painter->drawText(tr, label, opt);
}

void Icon::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Running" << d->exe << endl;
    QProcess::startDetached(d->exe);
    DesktopWidget::mouseDoubleClickEvent(event);
}

}
