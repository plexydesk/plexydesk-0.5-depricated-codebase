
#include "backdropfactory.h"

namespace PlexyDesk
{

class BackdropFactory::Private
{
    public:
        Private() {}
        ~Private() {}
};


BackdropFactory::BackdropFactory(QObject * parent):QObject(parent),d(new Private)
{

}

BackdropFactory::~BackdropFactory()
{
    delete d;
}


} // namespace PlexyDesk


#include "backdropfactory.moc"
