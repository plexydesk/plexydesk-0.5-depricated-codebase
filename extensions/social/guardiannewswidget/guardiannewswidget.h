#ifndef PLEXY_WIDGET_TWITTER_H
#define PLEXY_WIDGET_TWITTER_H

#include <plexy.h>
#include <abstractplugininterface.h>
#include <datainterface.h>
#include <widgetinterface.h>
#include "guardiannews.h"
#include "socialqdbusplugindata.h"
#include <widgetplugin.h>
#include <QScrollArea>
#include <QPixmap>
#include <QGraphicsItem>
#include <QPushButton>
#include <QLineEdit>
class QPushButton;
class QLineEdit;
class QWidget;
class QLabel;
class VISIBLE_SYM GuardianNews : public PlexyDesk::WidgetPlugin
{
    Q_OBJECT
    public:

        GuardianNews(QObject * object = 0);
        virtual ~GuardianNews();
        virtual QGraphicsItem * item();// {};

    public slots:
        void update();
        void bodyActivator(QString link);
    signals:
        void spinMe();
   private:
        QWidget * base;
        QList<QLabel *> newsItems;
        QLabel * settings;
        QLabel * newsParent;
        QLabel * bodyParent;
        QScrollArea * newsScroll;
        QString date,imagePath,category;
        QStringList headLine,body,trailText,webUrl,doc_Type,image_Url;
        QPushButton *adv,*submit,*back;
        QLineEdit * keyWord;
        bool reported;
        SocialQDBusPluginData * reporter;
        PlexyDesk::GuardianNewsWidget * widget;
        //-------------private methods
        void drawLabels();
        void drawBody(int i);
        void showCaptions();
        void resetBase();
        void dispAdvanceUser();



        QLabel * makeLabel(int x,int y, int w, int h, qreal opacityLevel);
        QLabel * makeLabel(int x, int y, int w, int h, QWidget * parent);
        QScrollArea * makeQScrollArea(int x, int y, int w, int h, bool HScrollBar,bool VScrollBar,QWidget *widget,bool setAsParent,bool setAsWidget);
        QScrollArea * makeQScrollArea(int x, int y, int w, int h, bool HScrollBar,bool VScrollBar,QWidget *widget,QWidget * parent);

   private slots :
        void setFilterType();
        void AdvanceMode();
        void submitSettings();
        void guardianSettings();
        void storeSettings(QString text);


};
#endif
