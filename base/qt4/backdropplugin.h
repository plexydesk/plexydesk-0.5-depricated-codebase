
#ifndef PLEXY_BACK_PLUGIN_H
#define PLEXY_BACK_PLUGIN_H

#include <plexy.h>
#include <config.h>
#include <baseplugin.h>

class QPainter;
class QVariant;
class QRectF;

namespace PlexyDesk
{
class VISIBLE_SYM BackdropPlugin : public BasePlugin
{
    Q_OBJECT
public :
    BackdropPlugin(QObject  * object = 0): BasePlugin(object) {}
    virtual ~BackdropPlugin() {}
    virtual void render(QPainter *p, QRectF r) = 0;
public slots:
    virtual void pushData(QVariant&) = 0;
signals:
    void data(QVariant&);
};
}
#endif
