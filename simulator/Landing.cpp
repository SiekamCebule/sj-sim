#include "Landing.h"

#include <QObject>

Landing::Landing(short type, double imbalance) : type(type),
    imbalance(imbalance)
{}

QString Landing::getTextLandingType() const
{
    switch(getType())
    {
    case TelemarkLanding: return "Telemark";
    case BothLegsLanding: return "Na dwie nogi";
    case SupportLanding: return "Podpórka";
    case Fall: return "Upadek";
    }
}

short Landing::getType() const
{
    return type;
}

void Landing::setType(short newType)
{
    type = newType;
}

double Landing::getImbalance() const
{
    return imbalance;
}

void Landing::setImbalance(double newImbalance)
{
    imbalance = newImbalance;
}

QString Landing::getShortLandingTypeTextInfo(short type)
{
    switch(type)
    {
    case Landing::TelemarkLanding:
        return QObject::tr("T");
    case Landing::BothLegsLanding:
        return QObject::tr("DN");
    case Landing::SupportLanding:
        return QObject::tr("P");
    case Landing::Fall:
        return QObject::tr("U");
    }
    return "ERROR";
}

QString Landing::getStyleSheetForLandingTypeTextInfo(short type)
{
    switch(type)
    {
    case Landing::TelemarkLanding:
        return "QLabel{color: rgb(23, 138, 71);}";
    case Landing::BothLegsLanding:
        return "QLabel{color: rgb(194, 209, 59);}";
    case Landing::SupportLanding:
        return "QLabel{color: rgb(214, 147, 64);}";
    case Landing::Fall:
        return "QLabel{color: rgb(196, 88, 67);}";
    }
    return "QLabel{color: rgb(0, 0, 0);}";
}
