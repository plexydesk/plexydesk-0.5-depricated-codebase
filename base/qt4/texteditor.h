#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <plexy.h>
#include <desktopwidget.h>

namespace PlexyDesk {

class TextEditor : public PlexyDesk::DesktopWidget
{
    Q_OBJECT
public:
    TextEditor(const QRectF &rect, QGraphicsObject *parent = 0);
    ~TextEditor();

    void setText(const QString &text);
    QString text() const;

private:
    class PrivateTextEditor;
    PrivateTextEditor *const d;

};
}

#endif // TEXTEDITOR_H
