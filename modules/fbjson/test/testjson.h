#include <QtTest/QtTest>

class TestJson: public QObject
{
    Q_OBJECT

public slots:
    void onFinished();
    void onInvalidFinished();
    void onSessionReady();

private slots:
    void loadSession();
    void loadJsons();
    void loadInvalidJson();

private:
    bool mFetchComplete;
    QEventLoop *mLoop;
    QSignalSpy *mSpy;
};
