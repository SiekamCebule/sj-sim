#include "Jumper.h"

Jumper::Jumper(const QString &name, const QString &surname, const QString &countryCode, JumperSkills * jumperSkills, ulong jumperSkillsID) : name(name),
    surname(surname),
    countryCode(countryCode),
    jumperSkills(jumperSkills),
    jumperSkillsID(std::move(jumperSkillsID))
{}

void Jumper::setJumperSkillsID(ulong ID)
{
    jumperSkillsID = ID;
}

QString Jumper::getName() const
{
    return name;
}

void Jumper::setName(const QString &newName)
{
    name = newName;
}

QString Jumper::getSurname() const
{
    return surname;
}

void Jumper::setSurname(const QString &newSurname)
{
    surname = newSurname;
}

QString Jumper::getNameAndSurname() const
{
    return name + " " + surname;
}

QString Jumper::getCountryCode() const
{
    return countryCode;
}

void Jumper::setCountryCode(const QString &newCountryCode)
{
    countryCode = newCountryCode;
}

JumperSkills *Jumper::getJumperSkills() const
{
    return jumperSkills;
}

void Jumper::setJumperSkills(JumperSkills *newJumperSkills)
{
    jumperSkills = newJumperSkills;
}

ulong Jumper::getJumperSkillsID() const
{
    return jumperSkillsID;
}
