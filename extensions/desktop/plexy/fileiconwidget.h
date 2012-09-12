#ifndef FILEICONWIDGET_H
#define FILEICONWIDGET_H

#include <plexy.h>
#include <desktopwidget.h>

class FileIconWidget : public PlexyDesk::DesktopWidget
{
    Q_OBJECT
public:
    enum FileType {
       TYPE_FILE,
       TYPE_FOLDER
     };

    explicit FileIconWidget(const QRectF &rect, QGraphicsObject *parent = 0);
    virtual ~FileIconWidget();

    void setType (FileType type);

};

#endif // FILEICONWIDGET_H
