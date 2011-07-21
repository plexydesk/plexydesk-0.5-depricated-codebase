#ifndef PLEXY_DEBUG_H
#define PLEXY_DEBUG_H

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <config.h>


static void plexyWindowsLogger(QtMsgType type, const char *msg)
{
    if (strcmp(BUILD_MODE, "Release") == 0) {
        return;
    }

    static QFile data(QDir::QDir::toNativeSeparators(
                QDir::tempPath() +
                QString("/plexydesk_log_%1.txt").arg(QDateTime::currentDateTime().toString())));
    data.close();
    if (!data.open(QFile::WriteOnly | QFile::Append)) {
        return;
    }
 
    static QTextStream stream(&data);

     switch (type) {
     case QtDebugMsg:
	 stream << QString("Debug:%1").arg(msg) << endl;
         stream.flush();
         break;
     case QtWarningMsg:
	 stream << QString("Warning:%1").arg(msg);
         break;
     case QtCriticalMsg:
	 stream << QString("Critical:%1").arg(msg);
         break;
     case QtFatalMsg:
	 stream << QString("Fatal:%1").arg(msg);
	 stream.flush();
         abort();
     }
}

#endif
