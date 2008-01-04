#include "baserender.h"

namespace PlexyDesk
{
    BaseRender::BaseRender(QWidget * parent , const QGLWidget * shareWidget , Qt::WindowFlags flag):
        QGLWidget(parent,shareWidget,flag)
    {
        setMouseTracking(true);
        //TODO
        //add sample buffer and pbuffer support flags here
    }

    BaseRender::~BaseRender()
    {

    }

   void  BaseRender::initializeGL()
    {

    }

    void BaseRender::resizeGL(int w,int h)
    {

    }

    void BaseRender::paintGL()
    {

    }

    void BaseRender::paintEvent(QPaintEvent *)
    {

    }

} // namespace PlexyDesk


#include "baserender.moc"
