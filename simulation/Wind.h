#ifndef WIND_H
#define WIND_H

#include "../utilities/ClassWithID.h"


class Wind : public ClassWithID
{
public:

    Wind(short direction = Null, double value = 0);
    Wind(double value);
    enum Direction{
        Null,
        Front,
        FrontLeft,
        FrontRight,
        Back,
        BackLeft,
        BackRight,
        Left,
        Right
    };

private:
    short direction;
    double value;

public:
    short getDirection() const;
    void setDirection(short newDirection);
    double getValue() const;
    void setValue(double newValue);
};

#endif // WIND_H
