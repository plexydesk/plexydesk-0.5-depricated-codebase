/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef PLEXYWINDOWS_H
#define PLEXYWINDOWS_H

#include <QtCore>
#include <QtGui>
#include <QWidget>

extern "C" {
#include <X11/Xlib.h>
#include <X11/extensions/Xdamage.h>
}

class PlexyWindows : public QWidget
{
  Q_OBJECT
public:
  PlexyWindows(Display*d, Window w, XWindowAttributes *attr, QWidget * parent = 0, Qt::WindowFlags f = 0);
virtual ~PlexyWindows(){}
virtual int devType() const;
virtual void setVisible(bool visible);
virtual QSize sizeHint() const;
virtual QSize minimumSizeHint() const;
virtual int heightForWidth(int ) const;
virtual QPaintEngine* paintEngine() const;

protected:
virtual bool event(QEvent* );
virtual void mousePressEvent(QMouseEvent* );
virtual void mouseReleaseEvent(QMouseEvent* );
virtual void mouseDoubleClickEvent(QMouseEvent* );
virtual void mouseMoveEvent(QMouseEvent* );
virtual void wheelEvent(QWheelEvent* );
virtual void keyPressEvent(QKeyEvent* );
virtual void keyReleaseEvent(QKeyEvent* );
virtual void focusInEvent(QFocusEvent* );
virtual void focusOutEvent(QFocusEvent* );
virtual void enterEvent(QEvent* );
virtual void leaveEvent(QEvent* );
virtual void paintEvent(QPaintEvent* );
virtual void moveEvent(QMoveEvent* );
virtual void resizeEvent(QResizeEvent* );
virtual void closeEvent(QCloseEvent* );
virtual void contextMenuEvent(QContextMenuEvent* );
virtual void tabletEvent(QTabletEvent* );
virtual void actionEvent(QActionEvent* );
virtual void dragEnterEvent(QDragEnterEvent* );
virtual void dragMoveEvent(QDragMoveEvent* );
virtual void dragLeaveEvent(QDragLeaveEvent* );
virtual void dropEvent(QDropEvent* );
virtual void showEvent(QShowEvent* );
virtual void hideEvent(QHideEvent* );
virtual bool x11Event(_XEvent* );
virtual void changeEvent(QEvent* );
virtual int metric(QPaintDevice::PaintDeviceMetric ) const;
virtual void inputMethodEvent(QInputMethodEvent* );

public:
virtual QVariant inputMethodQuery(Qt::InputMethodQuery ) const;

protected:
virtual bool focusNextPrevChild(bool next);
virtual void styleChange(QStyle& );
virtual void enabledChange(bool );
virtual void paletteChange(const QPalette& );
virtual void fontChange(const QFont& );
virtual void windowActivationChange(bool );
virtual void languageChange();

public:
virtual bool eventFilter(QObject* , QEvent* );

protected:
virtual void timerEvent(QTimerEvent* );
virtual void childEvent(QChildEvent* );
virtual void customEvent(QEvent* );
virtual void connectNotify(const char* signal);
virtual void disconnectNotify(const char* signal);
};

#endif // PLEXYWINDOWS_H
