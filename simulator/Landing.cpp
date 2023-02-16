#include "Landing.h"

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
