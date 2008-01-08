#include "baserender.h"
#include <QPaintEvent>
#include <QPainter>
#include <QtGui>
#include <QtOpenGL>

namespace PlexyDesk
{

  class BaseRender::Private
  {

  public:
    Private ()
    {
    }
     ~Private ()
    {
    }
    CacheDict dict;
    void drawPattern (QPainter * p, const QRect & r)
    {
      p->fillRect (r, QColor (255, 255, 255));

    }
    QImage image;
    int width;
    int height;

  };



  BaseRender::BaseRender (QWidget * parent, const QGLWidget * shareWidget,
			  Qt::WindowFlags flag):QGLWidget (QGLFormat (QGL::
								      SampleBuffers),
							   parent,
							   shareWidget, flag),
    d (new Private)
  {
    setMouseTracking (true);
    //TODO
    //add sample buffer and pbuffer support flags here
    d->width = 0;
    d->height = 0;
  }

  BaseRender::~BaseRender ()
  {
      delete d;
  }

  void BaseRender::resetViewport (int width, int height)
  {

      glViewport( 0, 0, (GLint)width, (GLint)height );
      glMatrixMode( GL_PROJECTION );
      glLoadIdentity();
      glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);

  }

  void BaseRender::resetOverlay (int width, int height)
  {
    QString text = tr ("Plexy Desk 1.0, Revolution Begins");
    QFontMetrics metrics = QFontMetrics (font ());
    int border = qMax (4, metrics.leading ());

    QRect rect =
      metrics.boundingRect (0, 0, width - 2 * border, int (height * 0.125),
			    Qt::AlignCenter | Qt::TextWordWrap, text);
    d->image =
      QImage (width, rect.height () + 2 * border,
	      QImage::Format_ARGB32_Premultiplied);
    d->image.fill (qRgba (0, 0, 0, 127));

    QPainter painter;
    painter.begin (&d->image);
    painter.setRenderHint (QPainter::TextAntialiasing);
    painter.setPen (Qt::white);
    painter.drawText ((width - rect.width ()) / 2, border,
		      rect.width (), rect.height (),
		      Qt::AlignCenter | Qt::TextWordWrap, text);

    painter.end ();

  }


  void BaseRender::initializeGL ()
  {
  }

  void BaseRender::resizeGL (int w, int h)
  {
    d->width = w;
    d->height= h;
    resetViewport (w, h);
    resetOverlay(w,h);
  }


  void BaseRender::paintEvent (QPaintEvent * event)
  {

    QPainter p;
    p.begin (this);
    p.setRenderHint (QPainter::Antialiasing);

    glPushAttrib (GL_ALL_ATTRIB_BITS);
    glMatrixMode (GL_PROJECTION);
    glPushMatrix ();
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix ();

    qglClearColor (QColor::fromCmykF (0.70, 1.0, 1.0, 0.0));
    glShadeModel (GL_SMOOTH);
    glEnable (GL_DEPTH_TEST);
    glEnable (GL_CULL_FACE);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glEnable (GL_MULTISAMPLE);
    static GLfloat lightPosition[4] = { 1.5, 5.0, 7.0, 1.0 };
    glLightfv (GL_LIGHT0, GL_POSITION, lightPosition);

    resetViewport(width(),height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   //glTranslatef( 0.0, 0.0, -3.0 );
   //glTranslated(0.0, 0.0, -10.0);
    glColor3f(1.0,1.0,1.0);
//TODO
//GL DRAW COMMANDS
    glPopAttrib ();
    glMatrixMode (GL_MODELVIEW);
    glPopMatrix ();
    glMatrixMode (GL_PROJECTION);
    glPopMatrix ();


    p.drawImage((width() - d->image.width())/2, 0, d->image);
    p.end ();
  }

}				// namespace PlexyDesk


#include "baserender.moc"
