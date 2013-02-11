#ifndef BBMOBILEUI_H
#define BBMOBILEUI_H

#include <plexy.h>
#include <bb/cascades/Application>

using namespace bb::cascades;

#include <QObject>

class BBMobileUi : public Application
{
    Q_OBJECT
public:
    BBMobileUi(int &argc, char **argv);
    ~BBMobileUi();

    void setupUI();

private:
    void addController(const QString &controllerName);

    class BBMobileUiPrivate;
    BBMobileUiPrivate *const d;
};

#endif // BBMOBILEUI_H
