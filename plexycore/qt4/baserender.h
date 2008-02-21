#ifndef PLEXY_DESK_BASE_RENDER_H
#define PLEXY_DESK_BASE_RENDER_H

#include <plexy.h>
#include <QGLWidget>
#include <QtCore>
#include <QImage>
#ifdef Q_WS_X11
#include <GL/gl.h>
#include <GL/glu.h>
#endif

namespace PlexyDesk
{

class VISIBLE_SYM BaseRender:public QGLWidget
{
    Q_OBJECT
 
public:
    typedef QMap<QString,QPixmap*> CacheDict;

    BaseRender(QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);

    virtual ~BaseRender();

    void resetViewport (int width, int height);

    void resetOverlay  (int width, int height);

protected:
    
    void initializeGL();

    void resizeGL(int width, int height);

//    void paintGL();

    void paintEvent(QPaintEvent *);

    void drawWallpaper(){};
private:
    class Private;
    Private * const d ;
};

} // namespace #ifndef
#endif
