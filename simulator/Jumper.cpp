#include "Jumper.h"
#include "../global/CountryFlagsManager.h"

#include <QObject>

Jumper::Jumper(const QString &name, const QString &surname, const QString &countryCode, const JumperSkills &skills) : name(name),
    surname(surname),
    countryCode(countryCode),
    jumperSkills(skills)
{
    if(countryCode.length() == 3)
        updateCountryFlagPixmap();
}

QPixmap Jumper::getFlagPixmap() const
{
    return flagPixmap;
}

void Jumper::setFlagPixmap(const QPixmap &newFlagPixmap)
{
    flagPixmap = newFlagPixmap;
}

void Jumper::updateCountryFlagPixmap()
{
    setFlagPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(getCountryCode())));
}

JumperSkills Jumper::getJumperSkills() const
{
    return jumperSkills;
}

JumperSkills *Jumper::getJumperSkillsPointer() const
{
    return const_cast<JumperSkills*>(&jumperSkills);
}

void Jumper::setJumperSkills(const JumperSkills &newJumperSkills)
{
    jumperSkills = newJumperSkills;
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
    updateCountryFlagPixmap();
}
