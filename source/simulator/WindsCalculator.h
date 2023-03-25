#ifndef WINDSCALCULATOR_H
#define WINDSCALCULATOR_H

#include "../utilities/ClassWithID.h"
#include "Wind.h"
#include "Hill.h"

#include <QVector>


class WindsCalculator
{
public:
    enum AverageWindType{
        Original,
        Improved
    };
    static Wind getAveragedWind(const QVector<Wind> &winds, short type);
};

#endif // WINDSCALCULATOR_H
