#include <QtTest/QtTest>

class TestIcon: public QObject
{
    Q_OBJECT

public slots:
    void onFinished();
    void onInvalidFinished();

private slots:
    void loadIcons();
    void loadInvalidIcon();
    void loadThemePackInit();

private:
    bool mFetchComplete;
};
