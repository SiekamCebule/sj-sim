#include "JumpData.h"

JumpData::JumpData(Jumper *jumper, Hill *hill) : jumper(jumper),
    hill(hill)
{}

void JumpData::reset()
{
    distance = points = gateCompensation = windCompensation = totalCompensation = 0;
    landing = Landing();
    judges.clear();
    conditionsInfo = ConditionsInfo();
    jumper = nullptr;
    hill = nullptr;
}

Jumper *JumpData::getJumper() const
{
    return jumper;
}

void JumpData::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}

Hill *JumpData::getHill() const
{
    return hill;
}

void JumpData::setHill(Hill *newHill)
{
    hill = newHill;
}

double JumpData::getJudgesPoints() const
{
    return judgesPoints;
}

void JumpData::setJudgesPoints(double newJudgesPoints)
{
    judgesPoints = newJudgesPoints;
}

ConditionsInfo JumpData::getConditionsInfo() const
{
    return conditionsInfo;
}

void JumpData::setConditionsInfo(const ConditionsInfo &newConditionsInfo)
{
    conditionsInfo = newConditionsInfo;
}

QVector<double> JumpData::getJudges() const
{
    return judges;
}

void JumpData::setJudges(const QVector<double> &newJudges)
{
    judges = newJudges;
}

Landing JumpData::getLanding() const
{
    return landing;
}

void JumpData::setLanding(const Landing &newLanding)
{
    landing = newLanding;
}

double JumpData::getTotalCompensation() const
{
    return totalCompensation;
}

void JumpData::setTotalCompensation(double newTotalCompensation)
{
    totalCompensation = newTotalCompensation;
}

double JumpData::getWindCompensation() const
{
    return windCompensation;
}

void JumpData::setWindCompensation(double newWindCompensation)
{
    windCompensation = newWindCompensation;
}

double JumpData::getGateCompensation() const
{
    return gateCompensation;
}

void JumpData::setGateCompensation(double newGateCompensation)
{
    gateCompensation = newGateCompensation;
}

double JumpData::getPoints() const
{
    return points;
}

void JumpData::setPoints(double newPoints)
{
    points = newPoints;
}

double JumpData::getDistance() const
{
    return distance;
}

void JumpData::setDistance(double newDistance)
{
    distance = newDistance;
}
