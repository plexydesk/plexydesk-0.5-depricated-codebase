#ifndef ICONWIDGETVIEW_H
#define ICONWIDGETVIEW_H

#include <scrollwidget.h>

class IconWidgetView : public PlexyDesk::ScrollWidget
{
    Q_OBJECT
public:
    explicit IconWidgetView(const QRectF &rect, QGraphicsObject *parent = 0);

    /** \brief Sets the Directory to display
      This method will load the Directory DataSource and pass the
      path variable as an argument for the DataSource
      */
    void setDirectoryPath(const QString &path);

private:
    class PrivateIconWidgetView;
    PrivateIconWidgetView *const d;

};

#endif // ICONWIDGETVIEW_H
