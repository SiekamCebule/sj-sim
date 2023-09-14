#include "WindsCalculator.h"

#include "../utilities/functions.h"

#include <QRandomGenerator>
#include <QDebug>
#include <functional>

Wind WindsCalculator::getAveragedWind(const QVector<Wind> & winds, short type)
{
    double percent = 100 / winds.count();
    double windAvg = 0;
    int i=0;

    for(const auto & wind : winds)
    {
        if(type == Original){
            if(wind.getDirection() == Wind::Front || wind.getDirection() == Wind::FrontSide)
                windAvg += wind.getStrength() * percent;
            else if(wind.getDirection() == Wind::Back || wind.getDirection() == Wind::BackSide)
                windAvg -= wind.getStrength() * percent;
        }
        else{
            if(wind.getDirection() == Wind::Front)
                windAvg += wind.getStrength() * percent;
            else if(wind.getDirection() == Wind::FrontSide)
                windAvg += wind.getStrength() * 0.5 * percent;
            else if(wind.getDirection() == Wind::BackSide)
                windAvg -= wind.getStrength() * 0.5 * percent;
            else if(wind.getDirection() == Wind::Back)
                windAvg -= wind.getStrength() * percent;
        }
        i++;
    }
    windAvg /= 100;
    windAvg = roundDoubleToTwoPlaces(windAvg);

    return Wind(windAvg);
}

double WindsCalculator::getWindCompensation(Wind avgWind, Hill *hill)
{
    double compensation = 0;

    if(avgWind.getDirection() == Wind::Back)
        compensation = (avgWind.getStrength() * hill->getPointsForBackWind());
    else if(avgWind.getDirection() == Wind::Front)
        compensation = (-(avgWind.getStrength() * hill->getPointsForFrontWind()));

    compensation = (roundDoubleToOnePlace(compensation));
    return compensation;
}

double WindsCalculator::getGateCompensation(int startGate, int actualGate, Hill *hill)
{
    return (startGate - actualGate) * hill->getPointsForGate();
}
