
#ifndef VIEW_LAYER_H
#define VIEW_LAYER_H

#include <config.h>

#include <plexy.h>
#include <desktopwidget.h>

namespace PlexyDesk
{
class VISIBLE_SYM ViewLayer : public  QObject
{
    Q_OBJECT
public:
    ViewLayer(QObject* object = 0);

    virtual ~ViewLayer();

    /** \brief Adds a DesktopWid to a given layer
    *  \param layerName is is the layer to add the item to, if the
    *  layer dose not exisit a new layer defined by the layerName is
    *  created.
    *  \param item is the widget we wish to add
    * */
    void addItem(const QString& layerName , DesktopWidget * item);

    void showLayer(const QString& layer);

private:
    typedef QList <DesktopWidget*> List;
    typedef QMap <QString, List*> Layer;

    Layer layer;
    List * currentList;
    class Private;
    Private * const d;
};
} // namespace PlexyDesk

#endif
