#include "ConditionsInfo.h"

ConditionsInfo::ConditionsInfo(Wind *wind, short gate, ulong windID) : wind(wind),
    gate(gate),
    windID(std::move(windID))
{}

ulong ConditionsInfo::getWindID() const
{
    return windID;
}

void ConditionsInfo::setWindID(ulong newWindID)
{
    windID = newWindID;
}

short ConditionsInfo::getGate() const
{
    return gate;
}

void ConditionsInfo::setGate(short newGate)
{
    gate = newGate;
}

Wind *ConditionsInfo::getWind() const
{
    return wind;
}

void ConditionsInfo::setWind(Wind *newWind)
{
    wind = newWind;
}
