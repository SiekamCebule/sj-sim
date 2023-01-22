#ifndef CONDITIONSINFO_H
#define CONDITIONSINFO_H

#include "../utilities/ClassWithID.h"
#include "Wind.h"


class ConditionsInfo : public ClassWithID
{
public:
    ConditionsInfo(Wind *wind = nullptr, short gate = 0, ulong windID = 0);
private:
    Wind * wind;
    short gate;

    ulong windID;

public:
    Wind *getWind() const;
    void setWind(Wind *newWind);
    short getGate() const;
    void setGate(short newGate);
    ulong getWindID() const;
    void setWindID(ulong newWindID);
};

#endif // CONDITIONSINFO_H
