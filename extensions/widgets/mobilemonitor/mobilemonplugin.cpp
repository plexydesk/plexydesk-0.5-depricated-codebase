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

#include "mobilemonplugin.h"
#include <pluginloader.h>
#include <desktopwidget.h>
#include <QDeclarativeContext>

#ifdef Q_WS_QPA
#include <bb/cascades/NavigationPane>
#endif


class MobileMonController::MobileMonControllerPrivate
{
public:
    MobileMonControllerPrivate(){};
    ~MobileMonControllerPrivate() {}
};

MobileMonController::MobileMonController(QObject * /*object*/) :
    mFrameParentitem(0),
    mNoteBase(0),
    d (new MobileMonControllerPrivate)
{
    if (connectToDataSource("bbconnengine")) {
        connect(dataSource(), SIGNAL(sourceUpdated(QVariantMap)), this, SLOT(onDataUpdated(QVariantMap)));
    }
}

MobileMonController::~MobileMonController()
{
    if (mFrameParentitem)
        delete mFrameParentitem;

    qDeleteAll(mPinWidgets);

    delete d;
}

QObject *MobileMonController::defaultView()
{
#ifdef Q_WS_QPA
       return NULL;
#endif

   if (mFrameParentitem == NULL) {
       mFrameParentitem = new PingAuthWidget(QRectF(0.0, 0.0, 640.0, 240.0));
       mFrameParentitem->enableDefaultBackground(true);
       mFrameParentitem->setController(this);
       mFrameParentitem->setLabelName("Ping Widget");

       connect (mFrameParentitem, SIGNAL(approvedToken(QString)), this, SLOT(setApprovedToken(QString)));
   }


   if (mNoteBase == NULL) {
       QRectF widgetRect = QRectF(0.0, 0.0, 640.0, 800.0);
       mNoteBase = new NoteBookBase(widgetRect);
       mNoteBase->setController(this);
       mNoteBase->setLabelName("Note Pad");
   }

   return mNoteBase;
}

void MobileMonController::revokeSession(const QVariantMap &args)
{
    mFrameParentitem->setContentImage(args["src"].toString());
}

void MobileMonController::handleDropEvent(PlexyDesk::AbstractDesktopWidget * /*widget*/, QDropEvent *event)
{
    if ( event->mimeData()->urls().count() >= 0 ) {
        const QString droppedFile = event->mimeData()->urls().value(0).toLocalFile();

        QFileInfo info(droppedFile);
        QPixmap droppedPixmap(droppedFile);

        if ( !info.isDir() && !droppedPixmap.isNull() ) {
            mFrameParentitem->setContentImage(droppedPixmap);

            if (viewport()) {
                PlexyDesk::AbstractDesktopView *view = qobject_cast<PlexyDesk::AbstractDesktopView*>(viewport());
                if (view)
                    view->sessionDataForController(controllerName(),"src", droppedFile);
            } else
                qDebug() << Q_FUNC_INFO << "Saving session Failed";
        }
    }
}

void MobileMonController::setViewRect(const QRectF &rect)
{
    if (mFrameParentitem)
        mFrameParentitem->setPos(rect.x(), rect.y());
    else {
        mFrameParentitem = new PingAuthWidget(QRectF(0.0, 0.0, 640.0, 240.0));
        mFrameParentitem->enableDefaultBackground(true);
        mFrameParentitem->setController(this);

        connect (mFrameParentitem, SIGNAL(approvedToken(QString)), this, SLOT(setApprovedToken(QString)));
    }
}

QStringList MobileMonController::actions() const
{
    QStringList rv;

    rv << "Sync";
    rv << "Add Note";
    rv << "Set Pin";

    return rv;
}

void MobileMonController::requestAction(const QString &actionName, const QVariantMap &args)
{
    if (actionName == "Set Pin") {
        PingAuthWidget *pingWidget = new PingAuthWidget(QRectF(0.0, 0.0, 640.0, 240.0));
        pingWidget->setController(this);
        pingWidget->setLabelName("Ping Widget");

        if(viewport()) {
            PlexyDesk::AbstractDesktopView *view = qobject_cast<PlexyDesk::AbstractDesktopView*>(viewport());

            if (view)
                view->scene()->addItem(pingWidget);
            pingWidget->show();
        }

        connect (pingWidget, SIGNAL(approvedToken(QString)), this, SLOT(setApprovedToken(QString)));
        mPinWidgets.append(pingWidget);

        float xpos = 0.0;

        if (viewport()) {
            PlexyDesk::AbstractDesktopView *view = qobject_cast<PlexyDesk::AbstractDesktopView*>(viewport());
            if (view)
            xpos = (view->width() - pingWidget->boundingRect().width()) / 2;
        }
        pingWidget->setPos(xpos, 10.0);
    }

    if (actionName == "Add Note" && mNoteBase) {
        mNoteBase->addNote("Title", "Add a Note here");
    }
}

bool MobileMonController::deleteWidget(PlexyDesk::AbstractDesktopWidget *widget)
{
    return FALSE;
}

void MobileMonController::setApprovedToken(const QString &token)
{
    if (dataSource()) {
        PlexyDesk::DataSource *source = dataSource();

        QVariant arg;
        QVariantMap dataMap;

        dataMap["key"] = token;

        arg.setValue(dataMap);

        source->setArguments(arg);
    }
}
