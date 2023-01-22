#ifndef JUMPDATA_H
#define JUMPDATA_H

#include "../utilities/ClassWithID.h"
#include "ConditionsInfo.h"


class JumpData : public ClassWithID
{
public:
    JumpData();

private:
    ConditionsInfo * conditionsInfo;

public:
};

#endif // JUMPDATA_H
