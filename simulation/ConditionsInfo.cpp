#include "ConditionsInfo.h"

#include <QRandomGenerator>
#include <QDebug>
#include <functional>

ConditionsInfo::ConditionsInfo(int gate, double windSensorsFaulty, const QVector<Wind> & winds ) :
    winds(winds),
    gate(gate),
    windSensorsFaulty(windSensorsFaulty)
{
    this->winds.push_back(Wind(Wind::Right, 8));
    this->winds.push_back(Wind(Wind::Right, 2.4));
    this->winds.push_back(Wind(Wind::Right, 2.6));
    this->winds.push_back(Wind(Wind::FrontRight, 2.09));
    this->winds.push_back(Wind(Wind::FrontRight, 1.4));
    this->winds.push_back(Wind(Wind::Front, 0.7));
    this->winds.push_back(Wind(Wind::Front, 0.3));
}

Wind ConditionsInfo::getAveragedWind()
{
    double percents[7] = {10, 10, 20, 20, 15, 15, 10};
    double windAvg = 0;
    int i=0;
    for(const auto & wind : winds)
    {
        if(wind.getDirection() == Wind::Front || wind.getDirection() == Wind::FrontLeft ||wind.getDirection() == Wind::FrontRight)
            windAvg += wind.getValue() * percents[i];
        else if((wind.getDirection() == Wind::Back || wind.getDirection() == Wind::BackLeft ||wind.getDirection() == Wind::BackRight))
            windAvg -= wind.getValue() * percents[i];
        i++;
    }
    windAvg /= 100;

    return Wind(windAvg);
}

double ConditionsInfo::getWindSensorsFaulty() const
{
    return windSensorsFaulty;
}

void ConditionsInfo::setWindSensorsFaulty(double newWindSensorsFaulty)
{
    windSensorsFaulty = newWindSensorsFaulty;
}

QVector<ulong> ConditionsInfo::getWindsIDs() const
{
    return windsIDs;
}

void ConditionsInfo::setWindsIDs(const QVector<ulong> &newWindsIDs)
{
    windsIDs = newWindsIDs;
}

void ConditionsInfo::setVectorWindID(int index, ulong newID)
{
    if(index < windsIDs.size())
        windsIDs.insert(index, newID);
    else
        windsIDs.replace(index, newID);
}

QVector<Wind> & ConditionsInfo::getWinds()
{
    return winds;
}

void ConditionsInfo::setWinds(const QVector<Wind> &newWinds)
{
    winds = newWinds;
}


int ConditionsInfo::getGate() const
{
    return gate;
}

void ConditionsInfo::setGate(int newGate)
{
    gate = newGate;
}

ulong ConditionsInfo::getWindID(int index) const
{
    return winds.at(index).getID();
}
