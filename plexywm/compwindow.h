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

        bool isWmRunning();

        void addWindow(Window id);
        bool registerWindowManager();
	bool registerCompositeManager();
	
    private:
        void init();//setups  Atoms, registering ..etc
        //utility 
        //data
        class Private;
        Private * const d;
};

#endif


