#include "JumperSkills.h"
#include "Jumper.h"

JumperSkills::JumperSkills(double takeoffPower, double takeoffTechnique, double flightTechnique, short flightStyle, double form, double landingStyle, const QSet<Characteristic> &characteristics, Jumper * jumper) : takeoffPower(takeoffPower),
    takeoffTechnique(takeoffTechnique),
    flightStyle(flightStyle),
    flightTechnique(flightTechnique),
    form(form),
    landingStyle(landingStyle)
{
    if(jumper != nullptr)
        jumperID = jumper->getID();

    setCharacteristics(characteristics);
}

void JumperSkills::checkSkillsLimits()
{
    if(getLandingStyle() < 0)
        setLandingStyle(0);
    else if(getLandingStyle() > 20)
        setLandingStyle(20);

    if(getFlightStyle() < 0)
        setFlightStyle(0);
    else if(getFlightStyle() > 3)
        setFlightStyle(3);
}

Jumper *JumperSkills::getJumper() const
{
    return jumper;
}

void JumperSkills::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}

double JumperSkills::getForm() const
{
    return form;
}

void JumperSkills::setForm(double newForm)
{
    form = newForm;
}

void JumperSkills::setJumperID(ulong ID)
{
    jumperID = ID;
}

double JumperSkills::getTakeoffPower() const
{
    return takeoffPower;
}

void JumperSkills::setTakeoffPower(double newTakeoffPower)
{
    takeoffPower = newTakeoffPower;
}

double JumperSkills::getTakeoffTechnique() const
{
    return takeoffTechnique;
}

void JumperSkills::setTakeoffTechnique(double newTakeoffTechnique)
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

double JumperSkills::getFlightTechnique() const
{
    return flightTechnique;
}

void JumperSkills::setFlightTechnique(double newFlightTechnique)
{
    flightTechnique = newFlightTechnique;
}

double JumperSkills::getLandingStyle() const
{
    return landingStyle;
}

void JumperSkills::setLandingStyle(double newLandingStyle)
{
    landingStyle = newLandingStyle;
}

QSet<Characteristic> JumperSkills::getCharacteristics() const
{
    return characteristics;
}

void JumperSkills::setCharacteristics(const QSet<Characteristic> &newCharacteristics)
{
    characteristics = newCharacteristics;
}
