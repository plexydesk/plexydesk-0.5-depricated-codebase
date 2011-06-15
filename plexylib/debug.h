
#include <QFile>
#include <QTextStream>


static QTextStream &winDebug()
{
    static QTextStream *stream = new QTextStream(stdout, QIODevice::ReadWrite);
    return *stream;
}

