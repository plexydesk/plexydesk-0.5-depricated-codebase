#include "accountmanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    new PlexyDesk::SocialAccountsManager(&app);
    return app.exec();
}
