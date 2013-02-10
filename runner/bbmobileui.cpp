#include "bbmobileui.h"

#include <themepackloader.h>

class BBMobileUi::BBMobileUiPrivate
{
public:
    BBMobileUiPrivate(){}
    ~BBMobileUiPrivate(){}

    PlexyDesk::ThemepackLoader *mThemeLoader;
};

BBMobileUi::BBMobileUi(int &argc, char **argv) :
    Application(argc, argv),
    d(new BBMobileUiPrivate)
{
    d->mThemeLoader = new PlexyDesk::ThemepackLoader("default", this);
}

BBMobileUi::~BBMobileUi()
{
    delete d;
}

