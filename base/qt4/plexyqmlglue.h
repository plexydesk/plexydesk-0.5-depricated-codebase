#ifndef PLEXYQMLGLUE_H
#define PLEXYQMLGLUE_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QDir>
#include <QtDeclarative/QDeclarativeEngine>

namespace PlexyDesk
{
class PlexyQmlGlueData;

class PlexyQmlGlue
{
public:
    PlexyQmlGlue();
    PlexyQmlGlue(const PlexyQmlGlue &);
    PlexyQmlGlue &operator=(const PlexyQmlGlue &);
    ~PlexyQmlGlue();

    static QDeclarativeEngine *qmlEngine();
    static QDeclarativeEngine *newQmlEngine();

    
private:
    QSharedDataPointer<PlexyQmlGlueData> data;
    static QDeclarativeEngine *engine;
};
}
#endif // PLEXYQMLGLUE_H
