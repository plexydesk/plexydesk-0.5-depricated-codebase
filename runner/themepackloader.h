#ifndef THEMEPACKLOADER_H
#define THEMEPACKLOADER_H

#include <QObject>
#include <QSettings>

class ThemepackLoader : public QObject
{
    Q_OBJECT
public:
    explicit ThemepackLoader(QObject *parent = 0);
    QStringList themes() const;
    QStringList qmlFilesFromTheme(const QString &themeName);

signals:
    void ready();

public slots:

private:
    void scanThemepackPrefix();

};

#endif // THEMEPACKLOADER_H
