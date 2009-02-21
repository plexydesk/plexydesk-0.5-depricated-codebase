#ifndef COM_WINDOW_H
#define COM_WINDOW_H

#include <QtCore>
#include <QtGui>

extern "C" {
#include <X11/Xlib.h>
}

class CompWindow : public QWidget
{
    Q_OBJECT
    public:
        CompWindow();
        ~CompWindow();

        void addWindow(Window id);
    private:
        void init();//setups  Atoms, registering ..etc
        //data
        class Private;
        Private * const d;
};

#endif
