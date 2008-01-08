#include "extensionfactory.h"


namespace PlexyDesk
{
template <class T>
    AbstractPluginLoader * ExtensionProducer<T>::createObject()
    {
        return new T;
    }

}


