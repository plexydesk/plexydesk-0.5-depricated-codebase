#include <QtCore>

class Tester : public QObject
{
    Q_OBJECT
    public:
        Tester(QObject *p =0);
    public slots:
        void connectionHandler();
        void sampleHandler(QString str);
    private:
        int Test;
};

