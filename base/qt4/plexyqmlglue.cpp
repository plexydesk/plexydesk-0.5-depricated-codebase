#include "plexyqmlglue.h"

#include <config.h>
#include <QtCore/QSharedData>
#include <QtCore/QDir>
#include <QtDeclarative/QDeclarativeImageProvider>
#include <QtDeclarative/QDeclarativeContext>

#include <plexyconfig.h>
#include <svgprovider.h>
#include <qmlsvgprovider.h>
#include <qmlpixmapprovider.h>

namespace PlexyDesk
{

QDeclarativeEngine *PlexyQmlGlue::engine = 0;

class PlexyQmlGlueData : public QSharedData {
public:
};

QDeclarativeEngine *PlexyQmlGlue::qmlEngine()
{
    if (engine == 0) {
        engine = newQmlEngine();
        return engine;
    } else {
        return engine;
    }
}

QDeclarativeEngine *PlexyQmlGlue::newQmlEngine()
{
        engine = new QDeclarativeEngine;
        engine->addImportPath(QDir::toNativeSeparators(
                    Config::getInstance()->plexydeskBasePath() + "/" + PLEXYQTIMPORTSDIR + "/"));
        engine->addImageProvider(QLatin1String("plexydesk"), new QmlPixmapProvider);
        engine->addImageProvider(QLatin1String("plexydesk_svgprovider"), new QmlSvgProvider);
        engine->rootContext()->setContextProperty("plexydeskconfig", Config::getInstance());
        return engine;
}

PlexyQmlGlue::PlexyQmlGlue() : data(new PlexyQmlGlueData)
{
}

PlexyQmlGlue::PlexyQmlGlue(const PlexyQmlGlue &rhs) : data(rhs.data)
{
}

PlexyQmlGlue &PlexyQmlGlue::operator=(const PlexyQmlGlue &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

PlexyQmlGlue::~PlexyQmlGlue()
{
    if(engine)
        delete engine;
}

}
