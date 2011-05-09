#include "testicon.h"
#include <plexyconfig.h>
#include <iconprovider.h>
#include <iconjob.h>

#include <themepackloader.h>


void TestIcon::loadIcons()
{
    mFetchComplete = false;
    PlexyDesk::IconProvider *iconprovider = new PlexyDesk::IconProvider();
    PlexyDesk::IconJobPtr job = iconprovider->requestIcon("utilities-terminal", "32"); 
    connect(job.data(), SIGNAL(finished()), this, SLOT(onFinished()));
    while (!mFetchComplete) {
        qApp->processEvents();
    }
}

void TestIcon::loadInvalidIcon()
{
    mFetchComplete = false;
    PlexyDesk::IconProvider *iconprovider = new PlexyDesk::IconProvider();
    PlexyDesk::IconJobPtr job = iconprovider->requestIcon("terminator2", "32"); 
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
    ThemepackLoader *themeLoader = new ThemepackLoader("default");
    QCOMPARE(themeLoader->wallpaper(), QString("logoless.png"));
    QCOMPARE(themeLoader->widgets("native").count() , 1);
}

QTEST_MAIN(TestIcon)
