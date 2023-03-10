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
#include <QByteArray>

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

QVector<Jumper> Jumper::getJumpersVectorFromJson(const QByteArray &bytes)
{
    QVector<Jumper> jumpers;

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(bytes, &error);
    if(error.error != QJsonParseError::NoError)
    {
        QMessageBox message(QMessageBox::Icon::Critical, "B????d przy wczytytywaniu zawodnik??w", "Nie uda??o si?? wczyta?? zawodnik??w z pliku userData/GlobalDatabase/globalJumpers.json\nTre???? b????du: " + error.errorString(), QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return jumpers;
    }
    QJsonObject object = document.object();
    QJsonValue value = object.value("jumpers");
    QJsonArray array = value.toArray();

    for(const auto & val : array)
    {
        QJsonObject obj = val.toObject();
        Jumper jumper;
        jumper.setName(obj.value("name").toString());
        jumper.setSurname(obj.value("surname").toString());
        jumper.setCountryCode(obj.value("country-code").toString());
        jumper.getJumperSkillsPointer()->setTakeoffPower(obj.value("takeoff-power").toDouble());
        jumper.getJumperSkillsPointer()->setTakeoffTechnique(obj.value("takeoff-technique").toDouble());
        jumper.getJumperSkillsPointer()->setFlightTechnique(obj.value("flight-technique").toDouble());
        jumper.getJumperSkillsPointer()->setFlightStyle(obj.value("flight-style").toDouble());
        jumper.getJumperSkillsPointer()->setLandingStyle(obj.value("landing-style").toDouble());
        jumper.getJumperSkillsPointer()->setForm(obj.value("form").toDouble());

        QJsonArray characteristicsArray = obj.value("characteristics").toArray();
        for(const auto & val : characteristicsArray){
            jumper.getJumperSkillsPointer()->insertCharacteristic(val.toObject().value("type").toString(), val.toObject().value("level").toDouble());
        }

        jumpers.push_back(jumper);
    }

    Jumper::setupJumpersFlagPixmaps(jumpers);
    return jumpers;
}

void Jumper::setupJumpersFlagPixmaps(QVector<Jumper> & jumpers)
{
    for(auto & jumper : jumpers)
    {
        jumper.setFlagPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumper.getCountryCode().toLower())));
    }
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
