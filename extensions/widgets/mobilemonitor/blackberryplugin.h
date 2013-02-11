#ifndef BLACKBERRYPLUGIN_H
#define BLACKBERRYPLUGIN_H

#include <plexy.h>
#include <controllerinterface.h>

class BlackBerryPlugin : public PlexyDesk::ControllerInterface
{
    Q_OBJECT
public:
    explicit BlackBerryPlugin(QObject *parent = 0);
    virtual ~BlackBerryPlugin();

    QObject *defaultView();

    virtual void revokeSession(const QVariantMap &args);

    virtual void setViewRect(const QRectF &rect);

    QStringList actions() const;

    void requestAction(const QString& actionName, const QVariantMap &args);

    void createAuthWidget();

    void createMainPage();
    void createNoteEditor();
public Q_SLOTS:
    void onPinCanceled();
    void onActionSetPin();
    void onSetClicked();
    void onEditorActionBack();
    void onAddNoteAction();
    void onNoteSave();

private:
    class BlackBerryPluginPrivate;
    BlackBerryPluginPrivate *const d;
};

#endif // BLACKBERRYPLUGIN_H
