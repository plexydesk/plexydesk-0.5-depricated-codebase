/*#include "sociotwitterwidget.h"
   #include ""

   SocioTwitter::SocioTwitter(QWidget *parent)
    : QWidget(parent), ui(new Ui::SocioTwitterClass)
   {
    ui->setupUi(this);
   }

   SocioTwitter::~SocioTwitter()
   {
    delete ui;
   }*/

#include <plexy.h>
#include "sociotwitterwidget.h"
#include "sociotwitterinterface.h"
#include <math.h>

#include <QMatrix>
#include <QPixmap>
#include <QPaintEvent>
#include <QPainter>
#include <QBitmap>
#include <QGraphicsSceneWheelEvent>
#include <QStyleOptionGraphicsItem>

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QTextBrowser>
#include <QtGui/QWidget>

namespace PlexyDesk
{

SocioTwitterwidget::SocioTwitterwidget(const QRectF &rect, QWidget *widget) :
    DesktopWidget(rect, widget)
{


    if (SocioTwitterwidget->objectName().isEmpty())
        SocioTwitterwidget->setObjectName(QString::fromUtf8("SocioTwitterClass"));
    SocioTwitterwidget->resize(272, 415);
    QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(SocioTwitterClass->sizePolicy().hasHeightForWidth());
    SocioTwitterwidget->setSizePolicy(sizePolicy);
    SocioTwitterwidget->setCursor(QCursor(Qt::ArrowCursor));
    SocioTwitterwidget->setAcceptDrops(true);
    SocioTwitterwidget->setWindowTitle(QString::fromUtf8("SocioTwitter"));
    SocioTwitterwidget->setAutoFillBackground(false);
    SocioTwitterwidget->setStyleSheet(QString::fromUtf8("QWidget{   \n"
             "background:transparent;\n"
             "transparent-color:black;\n"
             "background-color:qlineargradient(spread:pad, x1:0.604, y1:0.409, x2:0.193, y2:0.938, stop:0.233503 rgba(1, 0,         0,255))\n"
             "\n"
             "\n"
             "  }"));

    lineEdit = new QLineEdit(SocioTwitterwidget);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
    lineEdit->setGeometry(QRect(0, 370, 271, 41));
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());

    lineEdit->setSizePolicy(sizePolicy1);
    lineEdit->setFocusPolicy(Qt::StrongFocus);
    lineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {     \n"
             " border: 1px solid gray;     \n"
             " border-radius: 5px;     \n"
             " padding: 0 8px;     \n"
             " background: beige;     \n"
             " selection-background-color: darkgray;\n"
             "  }"));

    scrollArea = new QScrollArea(SocioTwitterwidget);
    scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
    scrollArea->setGeometry(QRect(0, 40, 271, 331));

    QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);

    sizePolicy2.setHorizontalStretch(10);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());

    scrollArea->setSizePolicy(sizePolicy2);
    scrollArea->setAcceptDrops(true);
    scrollArea->setAutoFillBackground(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidgetResizable(true);
    scrollArea->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    scrollAreaWidgetContents = new QWidget();
    scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 267, 327));

    graphicsView = new QGraphicsView(scrollAreaWidgetContents);
    graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
    graphicsView->setGeometry(QRect(10, 20, 61, 51));
    graphicsView->setFrameShadow(QFrame::Raised);
    graphicsView->setLineWidth(1);
    graphicsView->setMidLineWidth(0);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    graphicsView_2 = new QGraphicsView(scrollAreaWidgetContents);
    graphicsView_2->setObjectName(QString::fromUtf8("graphicsView_2"));
    graphicsView_2->setGeometry(QRect(10, 80, 61, 51));
    graphicsView_2->setFrameShape(QFrame::StyledPanel);

    graphicsView_3 = new QGraphicsView(scrollAreaWidgetContents);
    graphicsView_3->setObjectName(QString::fromUtf8("graphicsView_3"));
    graphicsView_3->setGeometry(QRect(10, 140, 61, 51));
    graphicsView_3->setFrameShape(QFrame::NoFrame);

    graphicsView_4 = new QGraphicsView(scrollAreaWidgetContents);
    graphicsView_4->setObjectName(QString::fromUtf8("graphicsView_4"));
    graphicsView_4->setGeometry(QRect(10, 200, 61, 51));
    graphicsView_4->setFrameShape(QFrame::StyledPanel);

    graphicsView_5 = new QGraphicsView(scrollAreaWidgetContents);
    graphicsView_5->setObjectName(QString::fromUtf8("graphicsView_5"));
    graphicsView_5->setGeometry(QRect(10, 260, 61, 51));
    graphicsView_5->setFrameShape(QFrame::NoFrame);

    textBrowser = new QTextBrowser(scrollAreaWidgetContents);
    textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
    textBrowser->setGeometry(QRect(80, 20, 181, 51));
    textBrowser->setFrameShape(QFrame::Box);

    textBrowser_2 = new QTextBrowser(scrollAreaWidgetContents);
    textBrowser_2->setObjectName(QString::fromUtf8("textBrowser_2"));
    textBrowser_2->setGeometry(QRect(80, 80, 181, 51));
    textBrowser_2->setFrameShape(QFrame::NoFrame);

    textBrowser_3 = new QTextBrowser(scrollAreaWidgetContents);
    textBrowser_3->setObjectName(QString::fromUtf8("textBrowser_3"));
    textBrowser_3->setGeometry(QRect(80, 140, 181, 51));
    textBrowser_3->setFrameShape(QFrame::NoFrame);

    textBrowser_4 = new QTextBrowser(scrollAreaWidgetContents);
    textBrowser_4->setObjectName(QString::fromUtf8("textBrowser_4"));
    textBrowser_4->setGeometry(QRect(80, 200, 181, 51));
    textBrowser_4->setFrameShape(QFrame::NoFrame);

    textBrowser_5 = new QTextBrowser(scrollAreaWidgetContents);
    textBrowser_5->setObjectName(QString::fromUtf8("textBrowser_5"));
    textBrowser_5->setGeometry(QRect(80, 260, 181, 51));
    textBrowser_5->setFrameShape(QFrame::NoFrame);

    scrollArea->setWidget(scrollAreaWidgetContents);

    pushButton = new QPushButton(SocioTwitterClass);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    pushButton->setGeometry(QRect(10, 10, 71, 41));

    QFont font;
    font.setBold(true);
    font.setWeight(75);

    pushButton->setFont(font);
    pushButton->setCursor(QCursor(Qt::PointingHandCursor));
    pushButton->setMouseTracking(false);
    pushButton->setFocusPolicy(Qt::StrongFocus);

    OPtions = new QPushButton(SocioTwitterClass);
    OPtions->setObjectName(QString::fromUtf8("OPtions"));
    OPtions->setEnabled(true);
    OPtions->setGeometry(QRect(80, 10, 71, 41));

    QPalette palette;
    QLinearGradient gradient(0.604, 0.409, 0.193, 0.938);

    gradient.setSpread(QGradient::PadSpread);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    gradient.setColorAt(0.233503, QColor(1, 0, 0, 255));
    QBrush brush(gradient);

    palette.setBrush(QPalette::Active, QPalette::Button, brush);
    QBrush brush1(QColor(169, 169, 169, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Highlight, brush1);
    QLinearGradient gradient1(0.604, 0.409, 0.193, 0.938);
    gradient1.setSpread(QGradient::PadSpread);
    gradient1.setCoordinateMode(QGradient::ObjectBoundingMode);
    gradient1.setColorAt(0.233503, QColor(1, 0, 0, 255));

    QBrush brush2(gradient1);
    palette.setBrush(QPalette::Inactive, QPalette::Button, brush2);
    palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush1);
    QLinearGradient gradient2(0.604, 0.409, 0.193, 0.938);

    gradient2.setSpread(QGradient::PadSpread);
    gradient2.setCoordinateMode(QGradient::ObjectBoundingMode);
    gradient2.setColorAt(0.233503, QColor(1, 0, 0, 255));

    QBrush brush3(gradient2);
    palette.setBrush(QPalette::Disabled, QPalette::Button, brush3);
    palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush1);
    OPtions->setPalette(palette);

    QFont font1;
    font1.setFamily(QString::fromUtf8("Sans Serif"));
    font1.setPointSize(10);
    font1.setBold(true);
    font1.setWeight(75);

    OPtions->setFont(font1);
    OPtions->setCursor(QCursor(Qt::PointingHandCursor));
    OPtions->setAutoDefault(false);
    OPtions->setDefault(false);
    OPtions->setFlat(false);

    retranslateUi(SocioTwitterClass);

    QMetaObject::connectSlotsByName(SocioTwitterClass);
}

/*
    void retranslateUi(QWidget *SocioTwitterClass)
    {
        scrollArea->setStyleSheet(QApplication::translate("SocioTwitterClass", "QScrollArea {\n"
   " border: 2px solid #8f8f91;     \n"
   " border-radius: 6px;      \n"
   " min-width: 25px; }", 0, QApplication::UnicodeUTF8));
        graphicsView->setStyleSheet(QApplication::translate("SocioTwitterClass", "QGraphicsView{\n"
   " border: 0.5px solid #8f8f91; \n"
   "border-color: #006699;    \n"
   " border-radius: 5px;       \n"
   " min-width: 25px; }", 0, QApplication::UnicodeUTF8));
        graphicsView_2->setStyleSheet(QApplication::translate("SocioTwitterClass", "QGraphicsView{\n"
   " border: 0.5px solid #8f8f91; \n"
   "border-color: #006699;    \n"
   " border-radius: 5px;       \n"
   " min-width: 25px; }\n"
   "", 0, QApplication::UnicodeUTF8));
        graphicsView_3->setStyleSheet(QApplication::translate("SocioTwitterClass", "QGraphicsView{\n"
   " border: 0.5px solid #8f8f91; \n"
   "border-color: #006699;    \n"
   " border-radius: 5px;       \n"
   " min-width: 25px; }", 0, QApplication::UnicodeUTF8));
        graphicsView_4->setStyleSheet(QApplication::translate("SocioTwitterClass", "QGraphicsView{\n"
   " border: 0.5px solid #8f8f91; \n"
   "border-color: #006699;    \n"
   " border-radius: 5px;       \n"
   " min-width: 25px; }", 0, QApplication::UnicodeUTF8));
        graphicsView_5->setStyleSheet(QApplication::translate("SocioTwitterClass", "QGraphicsView{\n"
   " border: 0.5px solid #8f8f91; \n"
   "border-color: #006699;    \n"
   " border-radius: 5px;       \n"
   " min-width: 25px; }", 0, QApplication::UnicodeUTF8));
        textBrowser->setStyleSheet(QApplication::translate("SocioTwitterClass", "QTextBrowser{\n"
   " border: 0.5px solid #8f8f91; \n"
   "border-color: #006699;    \n"
   " border-radius: 5px;       \n"
   " min-width: 25px; }", 0, QApplication::UnicodeUTF8));
        textBrowser_2->setStyleSheet(QApplication::translate("SocioTwitterClass", "QTextBrowser{\n"
   " border: 0.5px solid #8f8f91; \n"
   "border-color: #006699;    \n"
   " border-radius: 5px;       \n"
   " min-width: 25px; }", 0, QApplication::UnicodeUTF8));
        textBrowser_3->setStyleSheet(QApplication::translate("SocioTwitterClass", "QTextBrowser{\n"
   " border: 0.5px solid #8f8f91; \n"
   "border-color: #006699;    \n"
   " border-radius: 5px;       \n"
   " min-width: 25px; }", 0, QApplication::UnicodeUTF8));
        textBrowser_4->setStyleSheet(QApplication::translate("SocioTwitterClass", "QTextBrowser{\n"
   " border: 0.5px solid #8f8f91; \n"
   "border-color: #006699;    \n"
   " border-radius: 5px;       \n"
   " min-width: 25px; }", 0, QApplication::UnicodeUTF8));
        textBrowser_5->setStyleSheet(QApplication::translate("SocioTwitterClass", "QTextBrowser{\n"
   " border: 0.5px solid #8f8f91; \n"
   "border-color: #006699;    \n"
   " border-radius: 5px;       \n"
   " min-width: 25px; }", 0, QApplication::UnicodeUTF8));
        pushButton->setStyleSheet(QApplication::translate("SocioTwitterClass", "QPushButton {\n"
   " border: 1px solid #8f8f91;     \n"
   " border-radius: 6px;     \n"
   " background-image:url(blue.png);\n"
   " min-width: 25px; }", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("SocioTwitterClass", "Home", 0, QApplication::UnicodeUTF8));
        OPtions->setStyleSheet(QApplication::translate("SocioTwitterClass", "QPushButton {\n"
   " border: 1px solid #8f8f91;     \n"
   " border-radius: 3px;     \n"
   " background-image:url(blue.png);  \n"
   " min-width: 25px; }", 0, QApplication::UnicodeUTF8));
        OPtions->setText(QApplication::translate("SocioTwitterClass", "Options", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(SocioTwitterClass);
    } // retranslateUi

   };*/




void SocioTwitterwidget::setPath(QString str)
{
    prefix = str + "/";
}

void
SocioTwitterwidget::drawWidget()
{

}
}
