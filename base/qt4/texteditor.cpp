#include "texteditor.h"
#include <QGraphicsProxyWidget>
#include <QTextEdit>

namespace PlexyDesk
{

class TextEditor::PrivateTextEditor
{
public:
    PrivateTextEditor() {}
    ~PrivateTextEditor() {}

    QGraphicsProxyWidget *mProxyWidget;
    QTextEdit *mEditor;
};

TextEditor::TextEditor(const QRectF &rect, QGraphicsObject *parent) :
    PlexyDesk::DesktopWidget(rect, parent),
    d(new PrivateTextEditor)
{
    d->mProxyWidget = new QGraphicsProxyWidget(this);
    d->mEditor = new QTextEdit(0);
    d->mEditor->setStyleSheet("background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ffffff, stop: 1 #E5EAEE); border : 0");

    d->mProxyWidget->setWidget(d->mEditor);

    d->mProxyWidget->resize(rect.size());
    d->mProxyWidget->setMinimumSize(rect.size());

    d->mProxyWidget->show();
    d->mEditor->move(0.0, 0.0);
    d->mProxyWidget->setPos(0.0, 0.0);
    enableShadow(false);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    enableDefaultBackground(false);
}

TextEditor::~TextEditor()
{
    delete d;
}

void TextEditor::setText(const QString &text)
{
    d->mEditor->setText(text);
}

QString TextEditor::text() const
{
    if (d->mEditor) {
        return d->mEditor->toPlainText();
    }

    return QString();
}

}
