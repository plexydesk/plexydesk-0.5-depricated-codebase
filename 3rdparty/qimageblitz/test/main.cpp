#include "mainwindow.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QStringList args(app.arguments());
    if(args.count() > 1){
        for(int i=1; i < args.count(); ++i){
            BlitzMainWindow *mw = new BlitzMainWindow;
            mw->openFile(args.at(i));
            mw->resize(640, 480);
            mw->show();
        }
    }
    else{
        BlitzMainWindow *mw = new BlitzMainWindow;
        mw->resize(640, 480);
        mw->show();
    }
    return(app.exec());
}

