#ifndef PENDINGJOB_H
#define PENDINGJOB_H

#include <plexy.h>

#include <QtCore>

namespace PlexyDesk
{
class PendingJob : public QObject
{
    Q_OBJECT
public:
    Q_DISABLE_COPY(PendingJob)
    /**
    **/
    bool isError();
    /**
    **/
    QString errorName() const;
    /**
    **/
    QString errorMessage() const;
    /**
    **/
    void setFinished(bool status, QString &error, const QString &message);
Q_SIGNALS:
    void finished();
protected:
    PendingJob(QObject *parent);
private:
    class Private;
    Private *const d;
};
}
#endif // PENDINGJOB_H
