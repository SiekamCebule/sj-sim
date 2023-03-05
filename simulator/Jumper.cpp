#include "Jumper.h"
#include "../global/CountryFlagsManager.h"

#include <QObject>
#include <QDebug>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMessageBox>

Jumper::Jumper(const QString &name, const QString &surname, const QString &countryCode, const JumperSkills &skills) : name(name),
    surname(surname),
    countryCode(countryCode),
    jumperSkills(skills)
{
    if(countryCode.length() == 3)
        updateCountryFlagPixmap();
}

Jumper::~Jumper()
{
}

QJsonObject Jumper::getJumperJsonObject(Jumper * jumper, bool saveSkills, bool saveCharacteristics)
{
    QJsonObject object;
    object.insert("name", jumper->getName());
    object.insert("surname", jumper->getSurname());
    object.insert("country-code", jumper->getCountryCode().toUpper());
    if(saveSkills == true){
        object.insert("takeoff-power", jumper->getJumperSkills().getTakeoffPower());
        object.insert("takeoff-technique", jumper->getJumperSkills().getTakeoffTechnique());
        object.insert("flight-technique", jumper->getJumperSkills().getFlightTechnique());
        object.insert("flight-style", jumper->getJumperSkills().getFlightStyle());
        object.insert("landing-style", jumper->getJumperSkills().getLandingStyle());
        object.insert("form", jumper->getJumperSkills().getForm());
    }

    if(saveCharacteristics == true){
        QJsonArray characteristicsArray;
        for(const auto & characteristic : jumper->getJumperSkills().getCharacteristics())
        {
            QJsonObject characteristicObject;
            characteristicObject.insert("type", characteristic.getType());
            characteristicObject.insert("level", characteristic.getLevel());
            characteristicsArray.push_back(characteristicObject);
        }
        object.insert("characteristics", characteristicsArray);
    }

    return object;
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
