#ifndef PLEXY_YOUTUBE_H
#define PLEXY_YOUTUBE_H

#include <plexy.h>
#include <controllerinterface.h>
#include "youtubewidget.h"

class YouTube : public PlexyDesk::ControllerInterface
{
    Q_OBJECT

public:
    YouTube(QObject *object = 0);
    virtual ~YouTube();
    PlexyDesk::AbstractDesktopWidget *defaultView();
    void revokeSession(const QVariantMap &args);
    void setViewRect(const QRectF &rect);

private:
    PlexyDesk::YouTubeWidget *mWidget;
};

#endif
