#include "guardiannewswidget.h"
#include "socialqdbusplugindata.h"
#include <pluginloader.h>
#include <QDate>
#include <QPushButton>
#include <QLineEdit>
#include<QScrollArea>
#include <QLabel>
#include<QFont>
#include<QImage>
#include<QMenu>
#include<QTimer>
#include <QWidget>
#include<QPixmap>
#include <QDBusArgument>
#include <QtDBus/QtDBus>
#include <QLinearGradient>
#include <QColor>
#include <QBrush>
#include <QPalette>
#include <QMovie>

struct NewsData
{
    QString contentID;
    QString headLine;
    QString trailText;
    QString body;
    QString webUrl;
    QString docType;
    QString imageUrl;
};

QDBusArgument &operator<<(QDBusArgument &argument, const NewsData &news)
{
        argument.beginStructure();
        argument << news.contentID << news.headLine << news.trailText<< news.body<<news.webUrl<<news.docType<<news.imageUrl;
        argument.endStructure();
        return argument;
}
const QDBusArgument &operator>>(const QDBusArgument &argument, NewsData &news)
{
        argument.beginStructure();
        argument >> news.contentID >> news.headLine >> news.trailText >> news.body >> news.webUrl >> news.docType >> news.imageUrl;
        argument.endStructure();
        return argument;
}



GuardianNews::GuardianNews(QObject * object)
{
    imagePath = applicationDirPath()+"/share/plexy/skins/default/guardiannews/";
    base = new QWidget();
    base->resize(420,300);
    base->setBackgroundRole(QPalette::NoRole);
    base->setPalette(QColor(0,0,0,0.5));
    base->move(20,20);
    settings = this->makeLabel(10,10,440,320,base);
    reported = false;
    newsParent = this->makeLabel(10,10,416,630,1.0);
    newsScroll = this->makeQScrollArea(2,2,416,296,false,true,newsParent,base);
    //Default display activation
    //newsScroll->show();
    //update();
    //end default display

    back = new QPushButton("New Search");
    back->hide();
    //Activate the settings controls
    this->guardianSettings();
    //end activate
    widget =  new PlexyDesk::GuardianNewsWidget(QRectF(0, 0, 460,340), base);

}
GuardianNews::~GuardianNews(){}
void GuardianNews::update()
{
    QVariantMap publications;
    QString t_date = QString::number(QDate::currentDate().year())+QString::number(QDate::currentDate().month())+QString::number(QDate::currentDate().day());
    QVariantMap args;

    //if (date != t_date)
    //{
        date = t_date;
        args["needDay"] = QVariant(date);
        args["searchCategory"] = QVariant(category);
    //}
    publications = reporter->getData("guardianplugin","getnews",args);
    //unwrapping the QVariantMap
    QStringList content_ID = publications.keys();
    if (content_ID.size() != 0)
    {
        if (content_ID[0] != "Network Error")
        {
            for (int i=0;i<content_ID.length();i++)
            {
                QVariant news = publications[content_ID.at(i)];
                QDBusArgument qdArg = qvariant_cast<QDBusArgument>(news);
                NewsData data = qdbus_cast<NewsData>(qdArg);
                headLine<<data.headLine;
                trailText<<data.trailText;
                this->webUrl<<data.webUrl;
                body<<data.body;
                doc_Type<<data.docType;
                image_Url<<data.imageUrl;
            }
        }else{
            QList<QVariant> dataPkg;
            dataPkg<<publications[content_ID[0]];
            if (dataPkg[0].canConvert<QString>())
            {
                for (int i=0;i<10;i++)
                {
                    if (dataPkg[0].canConvert<QString>())
                    {
                        headLine<<dataPkg[0].value<QString>();
                        trailText<<dataPkg[0].value<QString>();
                        body<<dataPkg[0].value<QString>();
                        this->webUrl<<dataPkg[0].value<QString>();
                        doc_Type<<dataPkg[0].value<QString>();
                        image_Url<<dataPkg[0].value<QString>();
                    }
                }
            }
        }
        reported = true;
    }else
        reported = false;
    //unwrapping completed..
    content_ID.clear();

}
void GuardianNews::drawLabels()
{
    //qDebug()<<"drawing the labels";
    newsParent->clear();
    for (int i=0;i<newsItems.size();i++)
    {
        newsItems.at(i)->clear();
    }
    newsItems.clear();
    bodyParent = this->makeLabel(0,0,416,300,1.0);
    newsScroll->show();
    newsParent->show();
    int t =0,j;
    for (int i =0; i<headLine.size() ;i++)
    {
        int textLength = headLine.at(i).length();
        if (i ==0)
            newsItems.append(this->makeLabel(2,2,360,22*((textLength/50)+1),newsParent));
        else
            newsItems.append(this->makeLabel(2,t,360,22*((textLength/50)+1),newsParent));
        j = t;
        t = textLength;
        textLength = trailText.at(i).length();
        newsItems.append(this->makeLabel(20,22*((t/50)+1)+j,360,15*((textLength/60)+1),newsParent));
        newsItems.append(this->makeLabel(2,(15*((textLength/60)+1))+(22*((t/50)+1)+j),60,20,newsParent));
        newsItems.append(this->makeLabel(80,(15*((textLength/60)+1))+(22*((t/50)+1)+j),50,20,newsParent));
        newsItems.append(this->makeLabel(150,(15*((textLength/60)+1))+(22*((t/50)+1)+j),65,20,newsParent));
        t = (15*((textLength/60)+1))+(22*((t/50)+1)+j) + 20;
    }
    newsParent->resize(416,t+50);

    for (int i =0;i<newsItems.size();i+=5)
    {
        newsItems.at(i)->setWordWrap(true);
        newsItems.at(i)->setText(headLine[i/5]);

        newsItems.at(i+1)->setWordWrap(true);
        newsItems.at(i+1)->setAlignment(Qt::AlignJustify);
        newsItems.at(i+1)->setTextFormat(Qt::RichText);
        QString trail = trailText.at(i/5);
        trail.remove(0,trail.indexOf(":")+1);
        newsItems.at(i+1)->setText("<font size = 2>"+trail+"</font>");

        newsItems.at(i+2)->setTextFormat(Qt::RichText);
        newsItems.at(i+2)->setText("<font size = 2 color = red>read more : </font>");

        newsItems.at(i+3)->setTextFormat(Qt::RichText);
        newsItems.at(i+3)->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
        newsItems.at(i+3)->setOpenExternalLinks(false);
        connect(newsItems.at(i+3),SIGNAL(linkActivated(QString)),this,SLOT(bodyActivator(QString)));
        int page = i/5;
        newsItems.at(i+3)->setText("<a href="+QString::number(page+1)+"><font size=1 color=cyan> view now </font></a>");

        newsItems.at(i+4)->setTextFormat(Qt::RichText);
        newsItems.at(i+4)->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
        newsItems.at(i+4)->setOpenExternalLinks(true);
        QString link = webUrl.at(i/5);
        newsItems.at(i+4)->setText("<a href="+link+"><font size=1 color=cyan> view browser </font></a>");

    }
    for (int i=0;i<newsItems.size();i++)
        newsItems.at(i)->show();
    //back = new QPushButton("New Search");
    back->show();
    back->setStyleSheet("QPushButton{border: 2px solid gray;border-radius: 5px;padding: 0 8px;color:white}");
    back->move(10,t);
    back->resize(200,40);
    back->setParent(newsParent);
    back->show();
    connect(back,SIGNAL(clicked()),this,SLOT(guardianSettings()));
}
void GuardianNews::bodyActivator(QString link)
{
    if (link == "finish")
        this->showCaptions();
    else
    {
        int t = link.toInt();
        if(t == 0)
        {
        }else
        {
            this->drawBody(t-1);
        }
    }

}
void GuardianNews::drawBody(int i)
{
    int h;
    if (body.at(i) != ""){
        if (!QImage(image_Url.at(i)).isNull())
            h = 22*(((body.at(i).length())/60)+1)+QImage(image_Url.at(i)).height();
        else
            h = 22*(((body.at(i).length())/60)+1)+60;
    }
    else{
        h = 400;
    }
    newsScroll->hide();
    bodyParent = this->makeLabel(0,0,416,h,1.0);
    newsScroll = this->makeQScrollArea(2,2,416,296,false,true,bodyParent,base);
    newsScroll->setWidget(bodyParent);
    QLabel * body_h = this->makeLabel(10,10,376,50,bodyParent);
    body_h->setWordWrap(true);
    body_h->setText(headLine.at(i));
    QLabel * body_P = new QLabel();
    body_P->move(10,60);
    body_P->setParent(bodyParent);
    int local_y;
    if (image_Url.at(i) != "Invalid Content")
    {
        QPixmap image = QPixmap::fromImage(QImage(image_Url.at(i)));
        image = image.scaled(376,376,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        body_P->setPixmap(image);
        local_y = image.height()+80;
    }
    else
    {
        body_P->resize(376,50);
        body_P->setAlignment(Qt::AlignJustify);
        body_P->setTextFormat(Qt::RichText);
        body_P->setWordWrap(true);
        body_P->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
        body_P->setOpenExternalLinks(true);
        body_P->setText("<font size = 2 color=magenta>This news content is a "+doc_Type.at(i)+" type document.<br>No picture is available.<br>Please visit this <a href="+webUrl.at(i)+"><font color=cyan>link</font></a> to view content through default browser</font>");
        local_y = 120;
    }
    QLabel * body_b = this->makeLabel(10,local_y,376,h,bodyParent);
    body_b->setTextFormat(Qt::RichText);
    body_b->setAlignment(Qt::AlignJustify);
    body_b->setWordWrap(true);
    if (body.at(i) != "")
        body_b->setText("<font size=2>"+body.at(i)+"<br><a href=finish><font color=cyan>Back</font></a></font>");
    else{
        body_b->move(10,300);
        body_b->setText("<font size=2><a href=finish>Back</a></font>");
    }

    if (doc_Type.at(i) == "audio" || doc_Type.at(i) == "video")
    {
        QLabel * note = new QLabel();
        note->move(50,325);
        note->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
        note->setOpenExternalLinks(true);
        note->setParent(bodyParent);
        note->setAlignment(Qt::AlignHCenter);
        note->setText("<font size=2 color = red>This content is "+doc_Type.at(i)+". <br>The content is temporary unavailable. <br>Please visit the source to view the content<br><a href="+webUrl.at(i)+">click Here</a></font>");
    }
    newsScroll->show();

    connect(body_b,SIGNAL(linkActivated(QString)),this,SLOT(bodyActivator(QString)));
}
void GuardianNews::showCaptions()
{
    newsScroll->hide();
    newsScroll = this->makeQScrollArea(2,2,416,296,false,true,newsParent,base);
    newsScroll->show();
}
void GuardianNews::guardianSettings()
{
    back->hide();
    headLine.clear();
    body.clear();
    trailText.clear();
    webUrl.clear();
    doc_Type.clear();
    image_Url.clear();
    ///---------clearing
    newsScroll->hide();
    newsParent->hide();
    settings->show();
    category = "search";
    base->move(0,0);
    base->resize(460,340);
    QLabel * banner = this->makeLabel(0,-65,440,200,settings);
    QPixmap ban = QPixmap(imagePath+"banner.png");
    ban = ban.scaled(440,200,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    banner->setPixmap(ban);
    keyWord = new QLineEdit();
    QLabel * lbl = new QLabel();
    lbl->setTextFormat(Qt::RichText);
    lbl->setText("<font size = 2>Search category : </font>");
    lbl->setWordWrap(true);
    lbl->resize(105,60);
    lbl->move(20,102);
    lbl->setParent(settings);
    keyWord->setParent(settings);
    keyWord->setBackgroundRole(QPalette::Base);
    keyWord->move(125,123);
    keyWord->resize(150,20);
    keyWord->setStyleSheet("QLineEdit{border: 2px solid gray;border-radius: 5px;padding: 0 8px;}");

    submit = new QPushButton("submit");
    submit->setFlat(true);
    submit->move(285,123);
    submit->resize(120,20);
    submit->setParent(settings);
    submit->setStyleSheet("QPushButton{border: 2px solid gray;border-radius: 5px;padding: 0 8px; color:white;}");


    /*//configuring the advance settings
    adv = new QPushButton("Enable advance user mode");
    adv->setFlat(true);
    adv->move(20,300);
    adv->resize(300,20);
    adv->setParent(settings);
    adv->setStyleSheet("QPushButton{border: 2px solid gray;border-radius: 5px;padding: 0 8px;color:red}");

    connect(adv,SIGNAL(clicked()),this,SLOT(AdvanceMode()));*/
    connect(submit,SIGNAL(clicked()),this,SLOT(submitSettings()));
    connect(keyWord,SIGNAL(textChanged(QString)),this,SLOT(storeSettings(QString)));
}
void GuardianNews::AdvanceMode()
{
    adv->hide();
    submit->hide();
    //this->dispAdvanceUser();
}
void GuardianNews::submitSettings()
{
    /*
    QLabel *loading =this->makeLabel(0,00,200,200,settings);
//    QPixmap ld = QPixmap(imagePath+"loading.gif");
//    ld  = ld.scaled(100,100,Qt::KeepAspectRatio,Qt::SmoothTransformation);
//    qDebug()<<ld.isNull();
//    loading->setPixmap(ld);
    QMovie *ld = new QMovie(imagePath+"loading.gif");
    ld->setSpeed(200);
    ld->start();
    loading->setMovie(ld);
    loading->show();*/
    /*qDebug()<<keyWord->text();
    QString t = keyWord->text();
    if (t == "")
        category = "default";
    else
        category = t;*/
    this->update();
    if (reported)
    {
        settings->hide();
        newsParent->show();
        newsScroll->show();
        this->resetBase();
        this->drawLabels();
    }else
    {
        QLabel *error = this->makeLabel(30,300,300,100,settings);
        error->setTextFormat(Qt::RichText);
        error->setText("<font color=red>Invalid search catergory<br><a href=http://www.guardian.co.uk><font color=cyan>Guardian.co.uk</font></a> did not return "+category+" related data.<br>Please try another category</font>");
        error->show();
    }
    keyWord->setText("");

}
void GuardianNews::setFilterType()
{
}
void GuardianNews::dispAdvanceUser()
{
    QLabel *advParent = new QLabel();
    advParent->setParent(settings);
    advParent->move(0,00);
    advParent->resize(460,150);
    advParent->show();

    QLabel *advanceNotify = new QLabel("Advance user mode enabled");
    advanceNotify->setParent(advParent);
    advanceNotify->move(20,200);
    advanceNotify->show();

    QLabel *typeCommand = new QLabel("<font size=2>Type command : </font>");
    QLabel *filterCommand = new QLabel("<font size=2>Filter command : </font>");
    QLabel *setC_Command = new QLabel("<font size=2>Command : </font>");
    QLabel *setAPIKey = new QLabel("<font size=2>User defined API key : </font>");
    QLabel *APIKeyNotify = new QLabel("<font size=2 color=red>This API key should be a valid key. Only registered developer may change this.</font>");

    QLineEdit *T_CM = new QLineEdit();
    QLineEdit *F_CM = new QLineEdit();
    QLineEdit *C_CM = new QLineEdit();
    QLineEdit *API_K = new QLineEdit();

    typeCommand->move(20,212);
    filterCommand->move(20,227);
    setC_Command->move(20,242);
    setAPIKey->move(20,257);

    typeCommand->setTextFormat(Qt::RichText);
    filterCommand->setTextFormat(Qt::RichText);
    setC_Command->setTextFormat(Qt::RichText);
    setAPIKey->setTextFormat(Qt::RichText);

    typeCommand->setParent(advParent);
    filterCommand->setParent(advParent);
    setC_Command->setParent(advParent);
    setAPIKey->setParent(advParent);

    T_CM->move(200,210);
    F_CM->move(200,225);
    C_CM->move(200,240);
    API_K->move(200,255);

    T_CM->setParent(advParent);
    F_CM->setParent(advParent);
    C_CM->setParent(advParent);
    API_K->setParent(advParent);

    T_CM->setStyleSheet("QPushButton{border: 2px solid gray;border-radius: 5px;padding: 0 8px;}");
    C_CM->setStyleSheet("QPushButton{border: 2px solid gray;border-radius: 5px;padding: 0 8px;}");
    F_CM->setStyleSheet("QPushButton{border: 2px solid gray;border-radius: 5px;padding: 0 8px;}");
    API_K->setStyleSheet("QPushButton{border: 2px solid gray;border-radius: 5px;padding: 0 8px;}");

    submit = new QPushButton("Submit");
    submit->setFlat(true);
    submit->setText("<font size=2 color=white>Submit</font>");
    submit->move(300,350);
    submit->setStyleSheet("QPushButton{border: 2px solid gray;border-radius: 5px;padding: 0 8px;color:white}");
    connect(submit,SIGNAL(clicked()),this,SLOT(submitSettings()));
}
void GuardianNews::storeSettings(QString text)
{
    if (text == "")
        category = "default";
    else
        category = text;
}
/*void GuardianNews::dispAdvanceUser()
{
    QLabel *advParent = new QLabel();
    advParent->setParent(settings);
    advParent->move(0,00);
    advParent->resize(460,150);
    advParent->show();

    QLabel *advanceNotify = new QLabel("Advance user mode enabled");
    advanceNotify->setParent(base);
    advanceNotify->move(20,200);
    advanceNotify->show();

    QLabel *typeCommand = new QLabel("<font size=2>Type command : </font>");
    QLabel *filterCommand = new QLabel("<font size=2>Filter command : </font>");
    QLabel *setC_Command = new QLabel("<font size=2>Command : </font>");
    QLabel *setAPIKey = new QLabel("<font size=2>User defined API key : </font>");
    QLabel *APIKeyNotify = new QLabel("<font size=2 color=red>This API key should be a valid key. Only registered developer may change this.</font>");

    QLineEdit *T_CM = new QLineEdit();
    QLineEdit *F_CM = new QLineEdit();
    QLineEdit *C_CM = new QLineEdit();
    QLineEdit *API_K = new QLineEdit();

    typeCommand->move(20,212);
    filterCommand->move(20,227);
    setC_Command->move(20,242);
    setAPIKey->move(20,257);

    typeCommand->setTextFormat(Qt::RichText);
    filterCommand->setTextFormat(Qt::RichText);
    setC_Command->setTextFormat(Qt::RichText);
    setAPIKey->setTextFormat(Qt::RichText);

    typeCommand->setParent(base);
    filterCommand->setParent(base);
    setC_Command->setParent(base);
    setAPIKey->setParent(base);

    T_CM->move(200,210);
    F_CM->move(200,225);
    C_CM->move(200,240);
    API_K->move(200,255);

    T_CM->setParent(base);
    F_CM->setParent(base);
    C_CM->setParent(base);
    API_K->setParent(base);

    T_CM->setStyleSheet("QPushButton{border: 2px solid gray;border-radius: 5px;padding: 0 8px;}");
    C_CM->setStyleSheet("QPushButton{border: 2px solid gray;border-radius: 5px;padding: 0 8px;}");
    F_CM->setStyleSheet("QPushButton{border: 2px solid gray;border-radius: 5px;padding: 0 8px;}");
    API_K->setStyleSheet("QPushButton{border: 2px solid gray;border-radius: 5px;padding: 0 8px;}");

    submit = new QPushButton("Submit");
    submit->setFlat(true);
    submit->setText("<font size=2 color=white>Submit</font>");
    submit->move(300,350);
    submit->setStyleSheet("QPushButton{border: 2px solid gray;border-radius: 5px;padding: 0 8px;color:white}");
    connect(submit,SIGNAL(clicked()),this,SLOT(submitSettings()));
}
*/
void GuardianNews::resetBase()
{
    base->move(20,20);
    base->resize(420,300);
}
QGraphicsItem * GuardianNews::item()
{
  return widget;
}
QLabel * GuardianNews::makeLabel(int x,int y, int w, int h, qreal opacityLevel)
{
    QLabel * label = new QLabel;
    label->move(x,y);
    label->resize(w,h);
    label->setWindowOpacity(opacityLevel);
    return label;
}
QLabel * GuardianNews::makeLabel(int x, int y, int w, int h, QWidget * parent)
{
    QLabel * label = new QLabel;
    label->move(x,y);
    label->resize(w,h);
    label->setParent(parent);
    return label;
}
QScrollArea * GuardianNews::makeQScrollArea(int x, int y, int w, int h, bool HScrollBar,bool VScrollBar,QWidget *widget,bool setAsParent,bool setAsWidget)
{
    QScrollArea * area = new QScrollArea;
    area->move(y,x);
    area->resize(w,h);
    if (HScrollBar && VScrollBar)
    {
        area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }else if (!HScrollBar && VScrollBar) {
        area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }else if (HScrollBar && !VScrollBar) {
        area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }else if (!HScrollBar && !VScrollBar) {
        area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    if (setAsWidget)
        area->setWidget(widget);
    if (setAsParent)
        area->setParent(widget);
    return area;
}
QScrollArea * GuardianNews::makeQScrollArea(int x, int y, int w, int h, bool HScrollBar,bool VScrollBar,QWidget *widget,QWidget * parent)
{
    QScrollArea * area = new QScrollArea;
    area->move(y,x);
    area->resize(w,h);
    if (HScrollBar && VScrollBar)
    {
        area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }else if (!HScrollBar && VScrollBar) {
        area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }else if (HScrollBar && !VScrollBar) {
        area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }else if (!HScrollBar && !VScrollBar) {
        area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    area->setWidget(widget);
    area->setParent(parent);
    return area;
}

