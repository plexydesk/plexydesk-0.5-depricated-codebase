#ifndef PLEXY_DEBUG_H
#define PLEXY_DEBUG_H

#include <QDir>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDateTime>


static void plexyWindowsLogger(QtMsgType type, const char *msg)
{
    if (QString::compare(BUILD_MODE, "Release", Qt::CaseInsensitive) == 0
        || QString::compare(BUILD_MODE, "MinSizeRel", Qt::CaseInsensitive) == 0) {
        return;
    }

    static QFile data(QDir::toNativeSeparators(
                QDir::tempPath() +
                QString("/plexydesk_log_%1.txt").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy_hh-mm-ss"))));
    data.close();
    if (!data.open(QFile::WriteOnly | QFile::Append)) {
        return;
    }

    static QTextStream stream(&data);

    switch (type) {
    case QtDebugMsg:
        stream << QString("Debug: %1").arg(msg) << endl;
        stream.flush();
        break;
    case QtWarningMsg:
        stream << QString("Warning: %1").arg(msg) << endl;
        break;
    case QtCriticalMsg:
        stream << QString("Critical: %1").arg(msg) << endl;
        break;
    case QtFatalMsg:
        stream << QString("Fatal: %1").arg(msg) << endl;
        stream.flush();
        abort();
    }
}

#endif
