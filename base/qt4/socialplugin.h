
#ifndef PLEXY_SOCIAL_PLUGIN_H
#define PLEXY_SOCIAL_PLUGIN_H
#include <baseplugin.h>
#include <config.h>
#include <plexy.h>

namespace PlexyDesk
{
class PLEXYDESK_EXPORT SocialPlugin : public BasePlugin
{
    Q_OBJECT
public:
    SocialPlugin(QObject *object = 0)
    {
        Q_UNUSED(object);
    }

    virtual ~SocialPlugin()
    {
    }

public slots:
    virtual void pushSocial(QVariant &) = 0;
signals:
    void data(QVariant &);
};
}
#endif
