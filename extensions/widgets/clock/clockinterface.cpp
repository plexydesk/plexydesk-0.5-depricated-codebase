#include "clockinterface.h"
#include "clock.h"

QSharedPointer<PlexyDesk::ControllerInterface> ClockInterface::controller()
{
    QSharedPointer<PlexyDesk::ControllerInterface> obj =
            QSharedPointer<PlexyDesk::ControllerInterface>(new Clock(this), &QObject::deleteLater);

    return obj;
}

//Q_EXPORT_PLUGIN2(plexyclock, ClockInterface)
