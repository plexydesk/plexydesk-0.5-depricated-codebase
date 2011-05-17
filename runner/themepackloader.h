#ifndef THEMEPACKLOADER_H
#define THEMEPACKLOADER_H

#include <QObject>
#include <QSettings>
#include <QPoint>
#include <desktopwidget.h>

class ThemepackLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString themeName READ QString WRITE setThemeName)

public:
    explicit ThemepackLoader(const QString &themeName, QObject *parent = 0);

    void setThemeName(const QString &name);

    QString wallpaper();
    QStringList widgets(const QString &type);
    QString qmlFilesFromTheme(const QString &themeName);
    QPoint widgetPos(const QString &name);
    PlexyDesk::DesktopWidget::State widgetView(const QString &name);


signals:
    void ready();

public slots:

private:
    void scanThemepackPrefix();

    class ThemepackLoaderPrivate;
    ThemepackLoaderPrivate *const d;
};

#endif // THEMEPACKLOADER_H
