#include "pendingjob.h"

namespace PlexyDesk
{
class PendingJob::Private
{
public:
    Private() {
    }
    ~Private() {
    }
    bool done;
    bool iserror;
    QString error;
    QString message;
};

PendingJob::PendingJob(QObject *parent) : QObject(parent), d(new Private)
{
    d->done = false;
    d->iserror = false;
}

bool PendingJob::isError()
{
    return !d->iserror;
}

QString PendingJob::errorName() const
{
    return d->error;
}

QString PendingJob::errorMessage() const
{
    return d->message;
}

void PendingJob::setFinished(bool status, QString &error, const QString &message)
{
    d->done = status;
    d->iserror = status;
    d->error = error;
    d->message = message;
    emit finished();
}

}
