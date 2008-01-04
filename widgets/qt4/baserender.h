#ifndef PLEXY_DESK_BASE_RENDER_H
#define PLEXY_DESK_BASE_RENDER_H

#include <plexy.h>
#include <QGLWidget>

namespace PlexyDesk
{

class VISIBLE_SYM BaseRender:public QGLWidget
{
    Q_OBJECT
 
public:

    BaseRender(QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);
    
    virtual ~BaseRender();

    void setWallpaper(const QPixmap& wallpaper);

protected:
    
    void initializeGL();

    void resizeGL(int width, int height);

    void paintGL();

    void paintEvent(QPaintEvent *);
};

} // namespace #ifndef
#endif
