#ifndef WIND_H
#define WIND_H

#include "../utilities/ClassWithID.h"

class QString;

class Wind : public ClassWithID
{
public:

    Wind(short direction = Null, double value = 0);
    Wind(double value);
    enum Direction{
        Null,
        Back,
        BackSide,
        Side,
        FrontSide,
        Front
    };

private:
    short direction;
    double strength;

public:
    short getDirection() const;
    void setDirection(short newDirection);
    double getStrengthToAveragedWind() const;
    static QString getStringWindDirection(short direction, bool windPrefix);
    static QString getStyleSheetForAveragedWind(double avgWind);
    double getStrength() const;
    void setStrength(double newStrength);
};

#endif // WIND_H
