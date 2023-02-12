#include "Wind.h"

#include <QDebug>

short Wind::getDirection() const
{
    return direction;
}

void Wind::setDirection(short newDirection)
{
    direction = newDirection;
}

double Wind::getValue() const
{
    return value;
}

void Wind::setValue(double newValue)
{
    value = newValue;
}

double Wind::getValueToAveragedWind() const
{
    if(getDirection() == Front) return getValue();
    else if(getDirection() == Back) return -getValue();
    else return 0;
}

Wind::Wind(short direction, double value) : direction(direction),
    value(value)
{}

Wind::Wind(double value) : value(value)
{
    if(value == 0)
        direction = Null;
    else if(value > 0)
        direction = Front;
    else{
        direction = Back;
        this->value = -(this->value);
    }
}
