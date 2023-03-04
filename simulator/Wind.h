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
    double value;

public:
    short getDirection() const;
    void setDirection(short newDirection);
    double getValue() const;
    void setValue(double newValue);
    double getValueToAveragedWind() const;

    static QString getStringWindDirection(short direction, bool windPrefix);
    static QString getStyleSheetForAveragedWind(double avgWind);
};

#endif // WIND_H
