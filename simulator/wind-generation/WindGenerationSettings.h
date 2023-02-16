#ifndef WINDGENERATIONSETTINGS_H
#define WINDGENERATIONSETTINGS_H

#include "../Wind.h"
#include "../Characteristic.h"
#include "../../utilities/ClassWithCharacteristics.h"

#include <QVector>

class WindGenerationSettings : public ClassWithCharacteristics
{
public:
    WindGenerationSettings(short baseDirection = 0, double windDirectionInstability = 0, double baseWindStrength = 0, double windStrengthInstability = 0);
private:
    short baseDirection;
    double windDirectionInstability; // maksymalnie 10.00
    double baseWindStrength;
    double windStrengthInstability; // maksymalnie 10.00
public:
    short getBaseDirection() const;
    void setBaseDirection(short newBaseDirection);
    double getWindDirectionInstability() const;
    void setWindDirectionInstability(double newWindDirectionStability);
    double getBaseWindStrength() const;
    void setBaseWindStrength(double newBaseWindStrength);
    double getWindStrengthInstability() const;
    void setWindStrengthInstability(double newWindStrengthStability);
};

#endif // WINDGENERATIONSETTINGS_H
