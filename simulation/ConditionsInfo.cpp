#include "ConditionsInfo.h"

#include <QRandomGenerator>
#include <QDebug>
#include <functional>

ConditionsInfo::ConditionsInfo(int gate, double windSensorsFaulty, const QVector<Wind> & winds ) :
    winds(winds),
    gate(gate),
    windSensorsFaulty(windSensorsFaulty)
{
    this->winds = generateNewWinds();
}

Wind ConditionsInfo::getWindSensor(const Wind &wind)
{
    Wind sensor = wind;
    qDebug()<<sensor.getValue();
    return sensor;
}

QVector<Wind> ConditionsInfo::generateNewWinds()
{
    QVector<Wind> winds;
    winds.push_back(Wind(Wind::Null, 0));
    winds.push_back(Wind(Wind::Null, 0));
    winds.push_back(Wind(Wind::Null, 0));
    winds.push_back(Wind(Wind::Null, 0));
    winds.push_back(Wind(Wind::Null, 0));
    winds.push_back(Wind(Wind::Null, 0));
    winds.push_back(Wind(Wind::Null, 0));

    return winds;
}

const Wind &ConditionsInfo::getWindForActualDistance(int distance, Hill *hill)
{
    int i = 0;
    if(distance >= hill->getKPoint() * 0.10)
        i = i; //bez zmian
    if(distance >= hill->getKPoint() * 0.30)
        i++;
    if(distance >= hill->getKPoint() * 0.45)
        i++;
    if(distance >= hill->getKPoint() * 0.60)
        i++;
    if(distance >= hill->getKPoint() * 0.75)
        i++;
    if(distance >= hill->getKPoint() * 0.90)
        i++;
    if(distance >= hill->getKPoint() * 1.05)
        i++;

    return winds[i];
}

Wind ConditionsInfo::getAveragedWind()
{
    int percents[7] = {10, 10, 20, 20, 15, 15, 10};
    double windAvg = 0;
    int i=0;
    for(const auto & wind : winds)
    {
        windAvg += wind.getValue() * double(percents[i]);
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
