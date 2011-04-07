
#ifndef PLEXY_SOCIAL_PLUGIN_H
#define PLEXY_SOCIAL_PLUGIN_H
#include <baseplugin.h>
#include <config.h>
#include <plexy.h>

namespace PlexyDesk
{
class VISIBLE_SYM SocialPlugin : public BasePlugin
{
    Q_OBJECT
public:
    SocialPlugin(QObject *object = 0) {
    }
    virtual ~SocialPlugin() {
    }
public slots:
    virtual void pushSocial(QVariant &) = 0;
signals:
    virtual void data(QVariant &) = 0;
};
}
#endif
