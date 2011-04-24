#include <QtTest/QtTest>

class TestJson: public QObject
{
    Q_OBJECT

public slots:
    void onFinished();
    void onInvalidFinished();

private slots:
    void loadJsons();
    void loadInvalidJson();

private:
    bool mFetchComplete;
};
