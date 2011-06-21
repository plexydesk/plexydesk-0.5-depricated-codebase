
#include <QFile>
#include <QTextStream>


static void plexyWindowsLogger(QtMsgType type, const char *msg)
{
     switch (type) {
     case QtDebugMsg:
         fprintf(stdout, "Debug: %s\n", msg);
         break;
     case QtWarningMsg:
         fprintf(stdout, "Warning: %s\n", msg);
         break;
     case QtCriticalMsg:
         fprintf(stdout, "Critical: %s\n", msg);
         break;
     case QtFatalMsg:
         fprintf(stdout, "Fatal: %s\n", msg);
         abort();
     }
}
