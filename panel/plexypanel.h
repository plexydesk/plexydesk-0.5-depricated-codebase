#ifndef PLEXYPANEL_H
#define PLEXYPANEL_H

#include <QWidget>

class PlexyPanel : public QWidget
{
    Q_OBJECT
public:
    PlexyPanel(QWidget *parent = 0, Qt::WindowFlags f = 0);
    virtual ~PlexyPanel();
private slots:
    void switchLayer();
private:
    void setup();
};

#endif // PLEXYPANEL_H
