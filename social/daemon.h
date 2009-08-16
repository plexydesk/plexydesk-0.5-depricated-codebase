#include <QApplication>
#include <QDBusAbstractAdaptor>
#include <QObject>
class SocialDaemon : QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "local.socioplexy.Configuration")
    //Q_PROPERTY(QString display READ display)
public:
    SocialDaemon(QApplication *parent);
public slots:
    QString display();
};
