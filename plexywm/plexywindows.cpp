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

#include "plexywindows.h"

PlexyWindows::PlexyWindows(Display* dsp, Window win, XWindowAttributes* attr, QWidget *parent, Qt::WindowFlags f  )
                           :QWidget(parent, f)
{
qDebug()<<"Creating PlexyWindow";
XSelectInput(dsp, win, (PropertyChangeMask | EnterWindowMask | FocusChangeMask));
}

int PlexyWindows::devType() const {
return QWidget::devType();
}

void PlexyWindows::setVisible(bool visible) {
QWidget::setVisible(visible);
}

QSize PlexyWindows::sizeHint() const {
return QWidget::sizeHint();
}

QSize PlexyWindows::minimumSizeHint() const {
return QWidget::minimumSizeHint();
}

int PlexyWindows::heightForWidth(int width) const {
return QWidget::heightForWidth(width);
}

QPaintEngine* PlexyWindows::paintEngine() const {
return QWidget::paintEngine();
}

bool PlexyWindows::event(QEvent* e) {
return QWidget::event(e);
}

void PlexyWindows::mousePressEvent(QMouseEvent* e) {
QWidget::mousePressEvent(e);
}

void PlexyWindows::mouseReleaseEvent(QMouseEvent* e) {
QWidget::mouseReleaseEvent(e);
}

void PlexyWindows::mouseDoubleClickEvent(QMouseEvent* e) {
QWidget::mouseDoubleClickEvent(e);
}

void PlexyWindows::mouseMoveEvent(QMouseEvent* e) {
QWidget::mouseMoveEvent(e);
}

void PlexyWindows::wheelEvent(QWheelEvent* e) {
QWidget::wheelEvent(e);
}

void PlexyWindows::keyPressEvent(QKeyEvent* e) {
QWidget::keyPressEvent(e);
}

void PlexyWindows::keyReleaseEvent(QKeyEvent* e) {
QWidget::keyReleaseEvent(e);
}

void PlexyWindows::focusInEvent(QFocusEvent* e) {
QWidget::focusInEvent(e);
}

void PlexyWindows::focusOutEvent(QFocusEvent* e) {
QWidget::focusOutEvent(e);
}

void PlexyWindows::enterEvent(QEvent* e) {
QWidget::enterEvent(e);
}

void PlexyWindows::leaveEvent(QEvent* e) {
QWidget::leaveEvent(e);
}

void PlexyWindows::paintEvent(QPaintEvent* e) {
QWidget::paintEvent(e);
}

void PlexyWindows::moveEvent(QMoveEvent* e) {
QWidget::moveEvent(e);
}

void PlexyWindows::resizeEvent(QResizeEvent* e) {
QWidget::resizeEvent(e);
}

void PlexyWindows::closeEvent(QCloseEvent* e) {
QWidget::closeEvent(e);
}

void PlexyWindows::contextMenuEvent(QContextMenuEvent* e) {
QWidget::contextMenuEvent(e);
}

void PlexyWindows::tabletEvent(QTabletEvent* e) {
QWidget::tabletEvent(e);
}

void PlexyWindows::actionEvent(QActionEvent* e) {
QWidget::actionEvent(e);
}

void PlexyWindows::dragEnterEvent(QDragEnterEvent* e) {
QWidget::dragEnterEvent(e);
}

void PlexyWindows::dragMoveEvent(QDragMoveEvent* e) {
QWidget::dragMoveEvent(e);
}

void PlexyWindows::dragLeaveEvent(QDragLeaveEvent* e) {
QWidget::dragLeaveEvent(e);
}

void PlexyWindows::dropEvent(QDropEvent* e) {
QWidget::dropEvent(e);
}

void PlexyWindows::showEvent(QShowEvent*e ) {
QWidget::showEvent(e);
}

void PlexyWindows::hideEvent(QHideEvent* e) {
QWidget::hideEvent(e);
}

bool PlexyWindows::x11Event(_XEvent* e) {
return QWidget::x11Event(e);
}

void PlexyWindows::changeEvent(QEvent*e ) {
QWidget::changeEvent(e);
}

int PlexyWindows::metric(QPaintDevice::PaintDeviceMetric m ) const {
return QWidget::metric(m);
}

void PlexyWindows::inputMethodEvent(QInputMethodEvent* e) {
QWidget::inputMethodEvent(e);
}

QVariant PlexyWindows::inputMethodQuery(Qt::InputMethodQuery q) const {
return QWidget::inputMethodQuery(q);
}

bool PlexyWindows::focusNextPrevChild(bool next) {
return QWidget::focusNextPrevChild(next);
}

void PlexyWindows::styleChange(QStyle& s) {
QWidget::styleChange(s);
}

void PlexyWindows::enabledChange(bool b) {
QWidget::enabledChange(b);
}

void PlexyWindows::paletteChange(const QPalette& p) {
QWidget::paletteChange(p);
}

void PlexyWindows::fontChange(const QFont& f) {
QWidget::fontChange(f);
}

void PlexyWindows::windowActivationChange(bool b) {
QWidget::windowActivationChange(b);
}

void PlexyWindows::languageChange() {
QWidget::languageChange();
}

bool PlexyWindows::eventFilter(QObject* o , QEvent* e) {
return QObject::eventFilter(o, e);
}

void PlexyWindows::timerEvent(QTimerEvent* e) {
QObject::timerEvent(e);
}

void PlexyWindows::childEvent(QChildEvent* e) {
QObject::childEvent(e);
}

void PlexyWindows::customEvent(QEvent*e ) {
QObject::customEvent(e);
}

void PlexyWindows::connectNotify(const char* signal) {
QObject::connectNotify(signal);
}

void PlexyWindows::disconnectNotify(const char* signal) {
QObject::disconnectNotify(signal);
}

