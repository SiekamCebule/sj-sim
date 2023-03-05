#include "Wind.h"

#include <QDebug>
#include <QString>
#include <QObject>
#include "../global/GlobalAppSettings.h"

short Wind::getDirection() const
{
    return direction;
}

void Wind::setDirection(short newDirection)
{
    direction = newDirection;
}

double Wind::getStrengthToAveragedWind() const
{
    if(getDirection() == Front) return getStrength();
    else if(getDirection() == Back) return -getStrength();
    else return 0;
}

QString Wind::getStringWindDirection(short direction, bool windPrefix)
{
    QString text;
    switch(direction)
    {
    case Null: return "NULL";
    case Back: text = QObject::tr("tylni"); break;
    case BackSide: text = QObject::tr("tylno-boczny"); break;
    case Side: text = QObject::tr("boczny"); break;
    case FrontSide: text = QObject::tr("przednio-boczny"); break;
    case Front: text = QObject::tr("przedni"); break;
    }
    if(GlobalAppSettings::get()->getLanguageID() == GlobalAppSettings::Polish){
        if(windPrefix == true)
            text.insert(0, "Wiatr ");
        else text.replace(0, 1, text[0].toUpper());
    }

    return text;
}

QString Wind::getStyleSheetForAveragedWind(double avgWind)
{
    if(avgWind > 0)
        return "QLabel{color: rgb(74, 189, 68);}";
    else if(avgWind < 0)
        return "QLabel{color: rgb(214, 41, 28);}";
    else return "QLabel{color: rgb(50, 50, 50);}";

    return "QLabel{color: yellow;}";
}

Wind::Wind(short direction, double value) : direction(direction),
    strength(value)
{}

Wind::Wind(double value) : strength(value)
{
    if(value == 0)
        direction = Null;
    else if(value > 0)
        direction = Front;
    else{
        direction = Back;
        this->strength = -(this->strength);
    }
}

double Wind::getStrength() const
{
    return strength;
}

void Wind::setStrength(double newStrength)
{
    strength = newStrength;
}
