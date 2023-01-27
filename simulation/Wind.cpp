#include "Wind.h"

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

Wind::Wind(short direction, double value) : direction(direction),
    value(value)
{}

Wind::Wind(double value)
{
    if(value == 0)
    {
        direction = Null;
        this->value = 0;
    }
    else if(value > 0)
    {
        direction == Front;
        this->value = value;
    }
    else //if(value < 0)
    {
        direction = Back;
        this->value = !value;
    }
}
