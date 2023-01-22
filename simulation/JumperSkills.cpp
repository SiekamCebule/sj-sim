#include "JumperSkills.h"
#include "Jumper.h"

JumperSkills::JumperSkills(short takeoffPower, short takeoffTechnique, short flightStyle, short flightTechnique, short landingStyle, const QSet<QString> &characteristics, Jumper * jumper) : takeoffPower(takeoffPower),
    takeoffTechnique(takeoffTechnique),
    flightStyle(flightStyle),
    flightTechnique(flightTechnique),
    landingStyle(landingStyle),
    characteristics(characteristics)
{
    if(jumper != nullptr)
        jumperID = jumper->getID();
}

void JumperSkills::setJumperID(ulong ID)
{
    jumperID = ID;
}

short JumperSkills::getTakeoffPower() const
{
    return takeoffPower;
}

void JumperSkills::setTakeoffPower(short newTakeoffPower)
{
    takeoffPower = newTakeoffPower;
}

short JumperSkills::getTakeoffTechnique() const
{
    return takeoffTechnique;
}

void JumperSkills::setTakeoffTechnique(short newTakeoffTechnique)
{
    takeoffTechnique = newTakeoffTechnique;
}

short JumperSkills::getFlightStyle() const
{
    return flightStyle;
}

void JumperSkills::setFlightStyle(short newFlightStyle)
{
    flightStyle = newFlightStyle;
}

short JumperSkills::getFlightTechnique() const
{
    return flightTechnique;
}

void JumperSkills::setFlightTechnique(short newFlightTechnique)
{
    flightTechnique = newFlightTechnique;
}

short JumperSkills::getLandingStyle() const
{
    return landingStyle;
}

void JumperSkills::setLandingStyle(short newLandingStyle)
{
    landingStyle = newLandingStyle;
}

QSet<QString> JumperSkills::getCharacteristics() const
{
    return characteristics;
}

void JumperSkills::setCharacteristics(const QSet<QString> &newCharacteristics)
{
    characteristics = newCharacteristics;
}
