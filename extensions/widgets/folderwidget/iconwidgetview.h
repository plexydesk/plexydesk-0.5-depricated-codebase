#ifndef ICONWIDGETVIEW_H
#define ICONWIDGETVIEW_H

#include <scrollwidget.h>

class IconWidgetView : public PlexyDesk::ScrollWidget
{
    Q_OBJECT
public:
    explicit IconWidgetView(const QRectF &rect, QGraphicsObject *parent = 0);
    virtual ~IconWidgetView();

    void setDirectoryPath(const QString &path);

public Q_SLOTS:
    void onDirLoaded(const QString &path);
    void onClicked(const QModelIndex & index);
    void infoViewClicked();

private:
    class PrivateIconWidgetView;
    PrivateIconWidgetView *const d;
};

#endif // ICONWIDGETVIEW_H
