#include "JumperSkills.h"
#include "Jumper.h"

JumperSkills::JumperSkills(short takeoffPower, short takeoffTechnique, short flightStyle, short flightTechnique, short form, short landingStyle, const QSet<Characteristic> &characteristics, Jumper * jumper) : takeoffPower(takeoffPower),
    takeoffTechnique(takeoffTechnique),
    flightStyle(flightStyle),
    flightTechnique(flightTechnique),
    form(form),
    landingStyle(landingStyle),
    characteristics(characteristics)
{
    if(jumper != nullptr)
        jumperID = jumper->getID();
}

void JumperSkills::insertCharacteristic(const Characteristic &characteristic)
{
    characteristics.insert(characteristic);
}

void JumperSkills::insertCharacteristic(short level, const QString &type)
{
    characteristics.insert(Characteristic(level, type));
}

void JumperSkills::removeCharacteristic(const Characteristic &characteristic)
{
    characteristics.remove(characteristic);
}

void JumperSkills::removeCharacteristic(const QString &type)
{
    characteristics.remove(Characteristic(0, type));
}

short JumperSkills::getLevelOfCharacteristic(const QString &characteristicType)
{
    for(QSet<Characteristic>::iterator it = characteristics.begin(); it != characteristics.end(); it++)
    {
        if(it->getType() == characteristicType)
            return it->getLevel();
    }
}

short JumperSkills::getForm() const
{
    return form;
}

void JumperSkills::setForm(short newForm)
{
    form = newForm;
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

QSet<Characteristic> JumperSkills::getCharacteristics() const
{
    return characteristics;
}

void JumperSkills::setCharacteristics(const QSet<Characteristic> &newCharacteristics)
{
    characteristics = newCharacteristics;
}
