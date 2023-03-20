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

QString JumperSkills::getJumpsEqualityString(short jumpsEquality)
{
    QStringList strings = JumperSkills::getJumpsEqualityStringList();
    switch(jumpsEquality)
    {
    case -3:
        return strings.at(0);
    case -2:
        return strings.at(1);
    case -1:
        return strings.at(2);
    case 0:
        return strings.at(3);
    case 1:
        return strings.at(4);
    case 2:
        return strings.at(5);
    case 3:
        return strings.at(6);
    }
    return QObject::tr("Błąd");
}

QStringList JumperSkills::getJumpsEqualityStringList()
{
    return QStringList{
        QObject::tr("Bardzo nierówne"),
                QObject::tr("Nierówne"),
                QObject::tr("Trochę mniej równe"),
                QObject::tr("Średnio równe"),
                QObject::tr("Trochę bardziej równe"),
                QObject::tr("Równe"),
                QObject::tr("Bardzo równe")
    };
}

short JumperSkills::getJumpsEquality() const
{
    return jumpsEquality;
}

void JumperSkills::setJumpsEquality(short newJumpsEquality)
{
    jumpsEquality = newJumpsEquality;
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

    if(getJumpsEquality() > 7)
        setJumpsEquality(7);
    else if(getJumpsEquality() < 1)
        setJumpsEquality(1);
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
