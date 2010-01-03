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
#include "videoitem.h"
#include <QtCore>
#include <QtGui>

namespace PlexyDesk
{
VideoItem::VideoItem(const QRectF &rect, QWidget *widget): DesktopWidget(rect, widget)
{
    vid = new VPlayer();
    vid->setFileName("/home/siraj/dwhelper/Java_Everywhere.flv");
    connect(vid, SIGNAL(frameReady(QImage &)), this, SLOT(getImage(QImage &)));
    connect(vid, SIGNAL(videoDone()), this, SLOT(loop()));

    vidsurf = new QImage(320, 240, QImage::Format_RGB32);
    setCacheMode(NoCache);
    brush = QBrush(QColor(0, 0, 0));
    snapped = false;
    frameno = 0;
    setDockImage(QPixmap("/usr/share/plexy/skins/widgets/widget01/xine.png"));
    setFaceImage(QPixmap("/usr/share/plexy/skins/widgets/widget01/filmback.png"));

}

VideoItem::~VideoItem()
{
    if (vid) {
        delete vid;
    }

    if (vidsurf) {
        delete vidsurf;
    }
}
void VideoItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    if (vidsurf) {
        snap = QPixmap().fromImage(*vidsurf);;
        if (snapped == false)
            snapped = true;
        else
            snapped == false;
    }

    DesktopWidget::mouseDoubleClickEvent(event);
}

void VideoItem::loop()
{
    delete vid;
    vid = new VPlayer();
    vid->setFileName("/home/siraj/dwhelper/Java_Everywhere.flv");
}

void VideoItem::getImage(QImage & img)
{
    if (vidsurf) {
        delete vidsurf;
    }

    vidsurf = &img;
    // brush.setTextureImage(img);
    update();
    frameno++;

}

void VideoItem::paintExtFace(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget * widget)
{

    //p->setRenderHints(QPainter::SmoothPixmapTransform);
    p->save();
    p->setClipRect(e->exposedRect);
    //p->setOpacity(0.5);
    p->setRenderHint(QPainter::SmoothPixmapTransform, false);
    p->setRenderHint(QPainter::Antialiasing , false);
    p->setRenderHint(QPainter::HighQualityAntialiasing , false);
    if (vidsurf)
        p->drawImage(QRect(23, 47, 347, 200), *vidsurf);
    // p->fillRect(QRect(30,30,320,240), brush);
    p->restore();

}

void VideoItem::paintExtDockFace(QPainter *p, const QStyleOptionGraphicsItem * e , QWidget * widget)
{

    p->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    p->setPen(QColor(215, 255, 98));
    p->setFont(QFont("Bitstream Charter", 12));
    //  p->drawText(QRect(8,5,64,64), Qt::AlignCenter ,"Video" ) ;
    p->save();
    QPointF center = e->exposedRect.center();
    QTransform mat = QTransform();
    mat.translate(center.x(), center.y());
    mat.rotate(-9.44, Qt::ZAxis);
    mat.translate(-center.x(), -center.y());
    // mat.shear(0.02,-0.01);
    p->setTransform(mat);
    p->setOpacity(0.8);
    if (snapped == true)
        p->drawPixmap(15, 43, e->exposedRect.width() - 30, e->exposedRect.height() - 63, snap);
    p->restore();
}

QRectF VideoItem::boundingRect() const
{
    return rect();
}

} // namespace PlexyDesk

#include "videoitem.moc"
