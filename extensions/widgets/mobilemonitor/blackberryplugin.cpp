#include "blackberryplugin.h"

#include <bb/cascades/NavigationPane>
#include <bb/cascades/Page>
#include <bb/cascades/Container>
#include <bb/cascades/Button>
#include <bb/cascades/TextArea>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/LayoutOrientation>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/ActionItem>
#include <bb/cascades/TextAreaInputMode>
#include <bb/cascades/SubmitKey>
#include <bb/cascades/TextEditor>
#include <bb/cascades/TitleBar>

#include <bb/system/SystemDialog>

using namespace bb::cascades;

class BlackBerryPlugin::BlackBerryPluginPrivate
{
public:
    BlackBerryPluginPrivate() {}
    ~BlackBerryPluginPrivate() {}

    NavigationPane *mPaneView;

    /* PIN Wigget */
    Page *mPinPage;
    Container *mPinContainer;
    Container *mPinMainContainer;
    Label *mPinInfoLabel;
    TextArea *mPinText;
    Button *mPinOkButton;
    Button *mPinCancelButton;
    StackLayout *mPinButtonLayout;
    StackLayout *mMainLayout;
    TitleBar *mPinTitle;

    /* Main View */
    Page *mMainPage;
    Container *mMainContainer;
    ActionItem *mMainPinAction;
    ActionItem *mMainAddNoteAction;
    TitleBar *mMainTitleBar;

    /* Note Editor */
    Page *mNoteEditor;
    Container *mNoteContainer;
    TextArea *mNoteTextEditor;
    TextArea *mNoteTextSubject;
    TitleBar *mNoteTitleBar;
    ActionItem *mNoteBack;
    ActionItem *mNoteSave;

    /*system */
    bb::system::SystemDialog *mDialog;

};

BlackBerryPlugin::BlackBerryPlugin(QObject *parent) :
    PlexyDesk::ControllerInterface(parent),
    d (new BlackBerryPluginPrivate)
{

    if (connectToDataSource("bbconnengine")) {
        connect(dataSource(), SIGNAL(sourceUpdated(QVariantMap)), this, SLOT(onDataUpdated(QVariantMap)));
    }

    d->mDialog = new bb::system::SystemDialog(this);

    d->mPaneView = NavigationPane::create();
    d->mPaneView->setPeekEnabled(false);

    createAuthWidget();
    createMainPage();
    createNoteEditor();

    d->mPaneView->push (d->mNoteEditor);
    d->mPaneView->push (d->mMainPage);
    d->mPaneView->push (d->mPinPage);

}

BlackBerryPlugin::~BlackBerryPlugin()
{
    delete d;
}

void BlackBerryPlugin::createAuthWidget()
{
    d->mPinPage = Page::create();

    d->mPinTitle = new TitleBar();
    d->mPinTitle->setTitle("PlexyDesk: Desktop Connect");
    d->mPinPage->setTitleBar(d->mPinTitle);

    d->mPinMainContainer = Container::create();
    d->mPinPage->setContent(d->mPinMainContainer);

    d->mPinInfoLabel = new Label(d->mPinMainContainer);
    d->mPinText = new TextArea(d->mPinMainContainer);
    d->mPinInfoLabel->setText("Enter PIN to Authorize");

    d->mPinContainer = new Container(d->mPinMainContainer);

    d->mPinButtonLayout = new StackLayout();
    d->mPinButtonLayout->setOrientation(LayoutOrientation::LeftToRight);
    d->mPinContainer->setLayout (d->mPinButtonLayout);

    d->mPinContainer->add(Label::create());
    d->mPinCancelButton = new Button(d->mPinContainer);
    d->mPinCancelButton->setText("Cancel");
    d->mPinOkButton = new Button (d->mPinContainer);
    d->mPinOkButton->setText("Start Sync");

    connect(d->mPinCancelButton, SIGNAL(clicked()), this, SLOT(onPinCanceled()));
    connect(d->mPinOkButton, SIGNAL(clicked()), this, SLOT(onSetClicked()));
}

void BlackBerryPlugin::createMainPage()
{
    d->mMainPage = Page::create();

    d->mMainTitleBar = new TitleBar();
    d->mMainTitleBar->setTitle("PlexyDesk: Note List");
    d->mMainPage->setTitleBar(d->mMainTitleBar);

    d->mMainContainer = Container::create();
    d->mMainPage->setContent(d->mMainContainer);

    d->mMainPinAction = ActionItem::create()
            .title("Authorize")
            .image(Image("asset:///back.png"));
    d->mMainPage->addAction(d->mMainPinAction, ActionBarPlacement::OnBar);
    connect(d->mMainPinAction, SIGNAL(triggered()), this, SLOT(onActionSetPin()));

    d->mMainAddNoteAction = ActionItem::create()
            .title("Add Note")
            .image(Image("asset:///back.png"));
    d->mMainPage->addAction(d->mMainAddNoteAction, ActionBarPlacement::OnBar);
    connect(d->mMainAddNoteAction, SIGNAL(triggered()), this, SLOT(onAddNoteAction()));
}

void BlackBerryPlugin::createNoteEditor()
{
    d->mNoteEditor = Page::create();
    d->mNoteContainer = Container::create();
    d->mNoteEditor->setContent(d->mNoteContainer);

    d->mNoteTextSubject = new TextArea(d->mNoteContainer);
    d->mNoteTextSubject->setInputMode(TextAreaInputMode::Text);

    d->mNoteTextEditor = new TextArea(d->mNoteContainer);
    d->mNoteTextEditor->setInputMode(TextAreaInputMode::Text);
    d->mNoteTextEditor->setMinHeight(400.0);

    d->mNoteTitleBar = new TitleBar();
    d->mNoteTitleBar->setTitle("PlexyDesk: Note Editor");
    d->mNoteEditor->setTitleBar(d->mNoteTitleBar);

    d->mNoteBack = ActionItem::create()
            .title("Note List")
            .image(Image("asset:///back.png"));
    d->mNoteEditor->addAction(d->mNoteBack, ActionBarPlacement::OnBar);
    connect(d->mNoteBack, SIGNAL(triggered()), this, SLOT(onEditorActionBack()));

    d->mNoteSave = ActionItem::create()
            .title("Save Note")
            .image(Image("asset:///back.png"));
    d->mNoteEditor->addAction(d->mNoteSave, ActionBarPlacement::OnBar);
    connect(d->mNoteSave, SIGNAL(triggered()), this, SLOT(onNoteSave()));
}

void BlackBerryPlugin::onPinCanceled()
{
    d->mPaneView->pop();
}

void BlackBerryPlugin::onActionSetPin()
{
    d->mPaneView->push (d->mPinPage);
}

void BlackBerryPlugin::onSetClicked()
{
    if (dataSource()) {
        PlexyDesk::DataSource *source = dataSource();

        QVariant arg;
        QVariantMap dataMap;
        dataMap["key"] = d->mPinText->text();
        arg.setValue(dataMap);

        source->setArguments(arg);

        d->mPaneView->pop();
    }
}

void BlackBerryPlugin::onEditorActionBack()
{
    d->mPaneView->pop();
    d->mPaneView->push(d->mMainPage);
}

void BlackBerryPlugin::onAddNoteAction()
{
    d->mPaneView->pop();
    d->mPaneView->push(d->mNoteEditor);
}

void BlackBerryPlugin::onNoteSave()
{
    if (d->mNoteTextSubject->text().isEmpty()) {
        d->mDialog->setTitle("Error : Empty Subject");
        d->mDialog->setBody("Missing Subject: Add a subject to save the note");
        d->mDialog->show();
        return;
    }
}

QObject *BlackBerryPlugin::defaultView()
{
    return d->mPaneView;
}

void BlackBerryPlugin::revokeSession(const QVariantMap &args)
{
}

void BlackBerryPlugin::setViewRect(const QRectF &rect)
{
}

QStringList BlackBerryPlugin::actions() const
{
}

void BlackBerryPlugin::requestAction(const QString &actionName, const QVariantMap &args)
{
}
