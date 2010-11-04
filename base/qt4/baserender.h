/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  :
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/
#ifndef PLEXY_DESK_BASE_RENDER_H
#define PLEXY_DESK_BASE_RENDER_H

#include <plexy.h>

#include <QGLWidget>

#ifdef Q_WS_X11
#include <GL/gl.h>
#include <GL/glu.h>
#endif

class QPixmap;
class QString;

namespace PlexyDesk
{
    class PLEXYDESK_EXPORT BaseRender : public QGLWidget
    {
        Q_OBJECT
    public:
        typedef QMap<QString, QPixmap *> CacheDict;

        BaseRender(QWidget * parent = 0,
                   const QGLWidget *shareWidget = 0,
                   Qt::WindowFlags f = 0);
        virtual ~BaseRender();

        void resetViewport(int width, int height);
        void resetOverlay(int width, int height);

    protected:
        void initializeGL();
        void resizeGL(int width, int height);
        void paintEvent(QPaintEvent *event);

    private:
       class Private;
        Private * const d ;
    };

} // namespace #ifndef
#endif
