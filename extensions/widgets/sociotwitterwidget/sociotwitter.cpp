#include "sociotwitter.h"
#include "sociotwitterwidget.h"
#include "sociotwitterinterface.h"



SocioTwitter::SocioTwitter(QObject * object)
{

}

SocioTwitter::~SocioTwitter()
{

}

QGraphicsItem * SocioTwitter::item()
{
    return new PlexyDesk::SocioTwitterwidget();
}


