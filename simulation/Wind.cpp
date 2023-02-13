#include "Wind.h"

#include <QDebug>
#include <QString>

short Wind::getDirection() const
{
    return direction;
}

void Wind::setDirection(short newDirection)
{
    direction = newDirection;
}

QString Wind::getStringDirection(bool windPrefix) const
{
    QString text;
    switch(getDirection())
    {
    case Null: return "NULL";
    case Back: text = "tylni"; break;
    case BackSide: text = "tylno-boczny"; break;
    case Side: text = "boczny"; break;
    case FrontSide: text = "przednio-boczny"; break;
    case Front: text = "przedni"; break;
    }
    if(windPrefix == true)
        text.insert(0, "Wiatr ");
    else text.replace(0, 1, text[0].toUpper());

    return text;
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
