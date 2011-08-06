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

#include "testicon.h"
#include <plexyconfig.h>
#include <iconprovider.h>
#include <iconjob.h>
#include <desktopwidget.h>
#include <themepackloader.h>


void TestIcon::loadIcons()
{
    mFetchComplete = false;
    PlexyDesk::IconProvider *iconprovider = new PlexyDesk::IconProvider();
    PlexyDesk::IconJobPtr job = iconprovider->requestIcon("utilities", "32");
    connect(job.data(), SIGNAL(finished()), this, SLOT(onFinished()));
    while (!mFetchComplete) {
        qApp->processEvents();
    }
}

void TestIcon::loadInvalidIcon()
{
    mFetchComplete = false;
    PlexyDesk::IconProvider *iconprovider = new PlexyDesk::IconProvider();
    PlexyDesk::IconJobPtr job = iconprovider->requestIcon("terminal2", "32");
    connect(job.data(), SIGNAL(finished()), this, SLOT(onInvalidFinished()));
    while (!mFetchComplete) {
        qApp->processEvents();
    }
}

void TestIcon::onFinished()
{
  PlexyDesk::IconJob * icon = qobject_cast<PlexyDesk::IconJob *>(sender());

  QVERIFY(icon);

  QCOMPARE(icon->iconPixmap().isNull(), 0);
  QCOMPARE(icon->isError(), false);

  mFetchComplete = true;
}

void TestIcon::onInvalidFinished()
{
  PlexyDesk::IconJob * icon = qobject_cast<PlexyDesk::IconJob *>(sender());

  QVERIFY(icon);

  QCOMPARE(icon->iconPixmap().isNull(), 1);
  QCOMPARE(icon->isError(), true);

  mFetchComplete = true;
}

void TestIcon::loadThemePackInit()
{
    PlexyDesk::ThemepackLoader *themeLoader = new PlexyDesk::ThemepackLoader("default");

    QCOMPARE(themeLoader->wallpaper(),
            QDir::toNativeSeparators(
                PlexyDesk::Config::getInstance()->plexydeskBasePath() +
                "//share/plexy/themepack/default/resources/default.png"));

    QCOMPARE(themeLoader->widgets("native").count() , 1);

    QCOMPARE(themeLoader->qmlFilesFromTheme("weather") ,
            QDir::toNativeSeparators(
                 PlexyDesk::Config::getInstance()->plexydeskBasePath() +
                 "/share/plexy/themepack/default/weather/weather.qml"));
    Q_FOREACH(const QString &qmlWidget, themeLoader->widgets("QML")) {
       qDebug() <<  themeLoader->qmlFilesFromTheme(qmlWidget);
       PlexyDesk::DesktopWidget *parent = new PlexyDesk::DesktopWidget(QRectF(0,0,0,0));
       parent->qmlFromUrl(QUrl(themeLoader->qmlFilesFromTheme(qmlWidget)));
    }
}

QTEST_MAIN(TestIcon)
