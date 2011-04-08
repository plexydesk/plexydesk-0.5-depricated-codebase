#include <QApplication>
#include "plexypanel.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PlexyPanel *panel = new PlexyPanel();
    panel->show();
    return app.exec();
}
