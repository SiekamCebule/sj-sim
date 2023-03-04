#include "WindsInfo.h"

#include "../utilities/functions.h"

#include <QRandomGenerator>
#include <QDebug>
#include <functional>

WindsInfo::WindsInfo(const QVector<Wind> & winds ) :
    winds(winds)
{
}

Wind WindsInfo::getAveragedWind(short type)
{
    double percent = 100 / winds.count();
    double windAvg = 0;
    int i=0;
    for(const auto & wind : winds)
    {
        if(type == Original){
            if(wind.getDirection() == Wind::Front || wind.getDirection() == Wind::FrontSide)
                windAvg += wind.getValue() * percent;
            else if(wind.getDirection() == Wind::Back || wind.getDirection() == Wind::BackSide)
                windAvg -= wind.getValue() * percent;
        }
        else{
            if(wind.getDirection() == Wind::Front)
                windAvg += wind.getValue() * percent;
            else if(wind.getDirection() == Wind::FrontSide)
                windAvg += wind.getValue() * 0.5 * percent;
            else if(wind.getDirection() == Wind::BackSide)
                windAvg -= wind.getValue() * 0.5 * percent;
            else if(wind.getDirection() == Wind::Back)
                windAvg -= wind.getValue() * percent;
        }
        i++;
    }
    windAvg /= 100;
    windAvg = roundDoubleToTwoPlaces(windAvg);

    return Wind(windAvg);
}

QVector<ulong> WindsInfo::getWindsIDs() const
{
    return windsIDs;
}

void WindsInfo::setWindsIDs(const QVector<ulong> &newWindsIDs)
{
    windsIDs = newWindsIDs;
}

void WindsInfo::setVectorWindID(int index, ulong newID)
{
    if(index < windsIDs.size())
        windsIDs.insert(index, newID);
    else
        windsIDs.replace(index, newID);
}

QVector<Wind> & WindsInfo::getWinds()
{
    return winds;
}

void WindsInfo::setWinds(const QVector<Wind> &newWinds)
{
    winds = newWinds;
}

ulong WindsInfo::getWindID(int index) const
{
    return winds.at(index).getID();
}
