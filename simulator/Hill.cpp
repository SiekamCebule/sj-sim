#include "Hill.h"
#include "Landing.h"

#include "../global/CountryFlagsManager.h"

#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMessageBox>

Hill::Hill(const QString &name, const QString &countryCode, double KPoint, double HSPoint, double pointsForMeter, double pointsForKPoint, double pointsForFrontWind, double pointsForBackWind, double pointsForGate, double takeoffEffect, double flightEffect, double realHS, bool autoPointsForKPoint, bool autoPointsForMeter, bool autoPointsForBackWind) : name(name),
    countryCode(countryCode),
    KPoint(KPoint),
    HSPoint(HSPoint),
    pointsForMeter(pointsForMeter),
    pointsForKPoint(pointsForKPoint),
    pointsForFrontWind(pointsForFrontWind),
    pointsForBackWind(pointsForBackWind),
    pointsForGate(pointsForGate),
    takeoffEffect(takeoffEffect),
    flightEffect(flightEffect),
    realHS(realHS),
    autoPointsForKPoint(autoPointsForKPoint),
    autoPointsForMeter(autoPointsForMeter),
    autoPointsForBackWind(autoPointsForBackWind)
{
    setCharacteristics(characteristics);
    setRealHSByCharacteristic();
}

QJsonObject Hill::getHillJsonObject(Hill *hill, bool saveKAndHSPoint, bool savePointsInfo, bool saveSimulationParameters)
{
    QJsonObject object;
    object.insert("name", hill->getName());
    object.insert("country-code", hill->getCountryCode());
    if(saveKAndHSPoint == true)
    {
        object.insert("k-point", hill->getKPoint());
        object.insert("hs-point", hill->getHSPoint());
    }
    if(savePointsInfo == true)
    {
        object.insert("points-for-meter", hill->getPointsForMeter());
        object.insert("points-for-k-point", hill->getPointsForKPoint());
        object.insert("points-for-front-wind", hill->getPointsForFrontWind());
        object.insert("points-for-back-wind", hill->getPointsForBackWind());
        object.insert("points-for-gate", hill->getPointsForGate());
    }
    if(saveSimulationParameters == true)
    {
        object.insert("takeoff-effect", hill->getTakeoffEffect());
        object.insert("flight-effect", hill->getFlightEffect());
    }

    QJsonArray characteristicsArray;
    for(const auto & characteristic : hill->getCharacteristics())
    {
        QJsonObject characteristicObject;
        characteristicObject.insert("type", characteristic.getType());
        characteristicObject.insert("level", characteristic.getLevel());
        characteristicsArray.push_back(characteristicObject);
    }
    object.insert("characteristics", characteristicsArray);

    return object;
}

QVector<Hill> Hill::getHillsVectorFromJson(const QByteArray &bytes)
{
    QVector<Hill> hills;
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(bytes, &error);
    if(error.error != QJsonParseError::NoError)
    {
        QMessageBox message(QMessageBox::Icon::Critical, "B????d przy wczytytywaniu skoczni", "Nie uda??o si?? wczyta?? skoczni z pliku userData/GlobalDatabase/globalHills.json\nTre???? b????du: " + error.errorString(), QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return hills;
    }

    QJsonObject object = document.object();
    QJsonValue value = object.value("hills");
    QJsonArray array = value.toArray();
    for(const auto & val : array)
    {
        QJsonObject obj = val.toObject();
        Hill hill;
        hill.setName(obj.value("name").toString());
        hill.setCountryCode(obj.value("country-code").toString());
        hill.setKPoint(obj.value("k-point").toDouble());
        hill.setHSPoint(obj.value("hs-point").toDouble());

        if(obj.value("points-for-meter").toString() == "auto")
        {
            hill.setPointsForMeter(Hill::calculatePointsForMeter(hill.getKPoint()));
            hill.setAutoPointsForMeter(true);
        }
        else hill.setPointsForMeter(obj.value("points-for-meter").toDouble());

        if(obj.value("points-for-k-point").toString() == "auto")
        {
            hill.setPointsForKPoint(Hill::calculatePointsForKPoint(hill.getKPoint()));
            hill.setAutoPointsForKPoint(true);
        }
        else hill.setPointsForKPoint(obj.value("points-for-k-point").toDouble());

        hill.setPointsForGate(obj.value("points-for-gate").toDouble());
        hill.setPointsForFrontWind(obj.value("points-for-front-wind").toDouble());

        if(obj.value("points-for-back-wind").toString() == "auto")
        {
            hill.setPointsForBackWind(Hill::calculatePointsForBackWindBy21PercentsOfFrontWind(hill.getPointsForFrontWind()));
            hill.setAutoPointsForBackWind(true);
        }
        else hill.setPointsForBackWind(obj.value("points-for-back-wind").toDouble());

        hill.setTakeoffEffect(obj.value("takeoff-effect").toDouble());
        hill.setFlightEffect(obj.value("flight-effect").toDouble());

        QJsonArray characteristicsArray = obj.value("characteristics").toArray();
        for(const auto & val : characteristicsArray){
            hill.insertCharacteristic(val.toObject().value("type").toString(), val.toObject().value("level").toDouble());
        }

        hills.push_back(hill);
    }

    Hill::setupHillsFlagPixmaps(hills);
    return hills;
}

void Hill::setupHillsFlagPixmaps(QVector<Hill> &hills)
{
    for(auto & hill : hills)
    {
        hill.setFlagPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(hill.getCountryCode().toLower())));
    }
}

bool Hill::getAutoPointsForBackWind() const
{
    return autoPointsForBackWind;
}

void Hill::setAutoPointsForBackWind(bool newAutoPointsForBackWind)
{
    autoPointsForBackWind = newAutoPointsForBackWind;
}

bool Hill::getAutoPointsForMeter() const
{
    return autoPointsForMeter;
}

void Hill::setAutoPointsForMeter(bool newAutoPointsForMeter)
{
    autoPointsForMeter = newAutoPointsForMeter;
}

bool Hill::getAutoPointsForKPoint() const
{
    return autoPointsForKPoint;
}

void Hill::setAutoPointsForKPoint(bool newAutoPointsForKPoint)
{
    autoPointsForKPoint = newAutoPointsForKPoint;
}

QPixmap Hill::getFlagPixmap() const
{
    return flagPixmap;
}

void Hill::setFlagPixmap(const QPixmap &newFlagPixmap)
{
    flagPixmap = newFlagPixmap;
}

double Hill::calculatePointsForMeter(double KPoint)
{
    if(KPoint <= 24)
        return 4.8;
    else if(KPoint <= 29)
        return 4.4;
    else if(KPoint <= 34)
        return 4.0;
    else if(KPoint <= 39)
        return 3.6;
    else if(KPoint <= 49)
        return 3.2;
    else if(KPoint <= 59)
        return 2.8;
    else if(KPoint <= 69)
        return 2.4;
    else if(KPoint <= 79)
        return 2.2;
    else if(KPoint <= 99)
        return 2.0;
    else if(KPoint <= 169)
        return 1.8;
    else if(KPoint >= 170)
        return 1.2;
    else return 0;
}

double Hill::calculatePointsForKPoint(double KPoint)
{
    if(KPoint < 170)
        return 60;
    else return 120;
}

double Hill::calculatePointsForBackWindBy21PercentsOfFrontWind(double pointsForFrontWind)
{
    return pointsForFrontWind * 1.21;
}

double Hill::calculateBestTakeoffHeightLevel(Hill *hill)
{
    double level = 1.075;
    if((hill->getTakeoffEffect() / (hill->getFlightEffect() / 1.00)) > 1)
        level += (hill->getTakeoffEffect() / (hill->getFlightEffect() / 0.85));
    else
        level += -(hill->getFlightEffect() / (hill->getTakeoffEffect() / 0.85));

    return level;
}

double Hill::calculateBestFlightHeightLevel(Hill *hill)
{
    double level = 0.7;
    if((hill->getTakeoffEffect() / (hill->getFlightEffect() / 1.00)) > 1)
        level += (hill->getTakeoffEffect() / (hill->getFlightEffect() / 0.85));
    else
        level += -(hill->getFlightEffect() / (hill->getTakeoffEffect() / 0.85));

    return level;
}

double Hill::getPointsForKPoint() const
{
    return pointsForKPoint;
}

void Hill::setPointsForKPoint(double newPointsForKPoint)
{
    pointsForKPoint = newPointsForKPoint;
}

double Hill::getRealHS() const
{
    return realHS;
}

void Hill::setRealHS(double newRealHS)
{
    realHS = newRealHS;
}

void Hill::setRealHSByCharacteristic()
{
    realHS = HSPoint;
    realHS *= 1 + (getLevelOfCharacteristic("real-hs-point") * 0.0145);
}

double Hill::getFlightEffect() const
{
    return flightEffect;
}

void Hill::setFlightEffect(double newFlightEffect)
{
    flightEffect = newFlightEffect;
}

double Hill::getTakeoffEffect() const
{
    return takeoffEffect;
}

void Hill::setTakeoffEffect(double newTakeoffEffect)
{
    takeoffEffect = newTakeoffEffect;
}

void Hill::setPointsForMeter(double newPointsForMeter)
{
    pointsForMeter = newPointsForMeter;
}


double Hill::getKAndRealHSDifference()
{
    setRealHSByCharacteristic();
    return getRealHS() - getKPoint();
}

double Hill::getLandingChanceChangeByHillProfile(double distance, short landingType)
{
    if(distance < getKPoint())
        return 0;
    if(landingType == Landing::TelemarkLanding)
    {
        if(distance < getKPoint() + getKAndRealHSDifference() * 0.5)
            return (distance - getKPoint()) / ((getKAndRealHSDifference() / 1.1) / 12);
        else if(distance <= getRealHS())
            return (distance - getKPoint()) / ((getKAndRealHSDifference() / 1.1) / 14);
        else if (distance < getRealHS() * 1.12)
            return (distance - getRealHS()) / ((getKAndRealHSDifference() / 1.1) / 260);
        else
            return (distance - getRealHS()) / ((getKAndRealHSDifference() / 1.1) / 300);
    }
    else if(landingType == Landing::BothLegsLanding)
    {
        if(distance < getKPoint() + getKAndRealHSDifference() * 0.5)
            return (distance - getKPoint()) / ((getKAndRealHSDifference() / 1.1) / 4);
        else if(distance <= getRealHS())
            return (distance - getKPoint()) / ((getKAndRealHSDifference() / 1.1) / 5);
        else if (distance < getRealHS() * 1.12)
            return (distance - getRealHS()) / ((getKAndRealHSDifference() / 1.1) / 255);
        else
            return (distance - getRealHS()) / ((getKAndRealHSDifference() / 1.1) / 1);
    }
    else if(landingType == Landing::SupportLanding)
    {
        if(distance < getKPoint() + getKAndRealHSDifference() * 0.5)
            return (distance - getKPoint()) / ((getKAndRealHSDifference() / 1.1) / 0.008);
        else if(distance <= getRealHS())
            return (distance - getKPoint()) / ((getKAndRealHSDifference() / 1.1) / 0.02);
        else if (distance < getRealHS() * 1.12)
            return (distance - getRealHS()) / ((getKAndRealHSDifference() / 1.1) / 40);
        else
            return (distance - getRealHS()) / ((getKAndRealHSDifference() / 1.1) / 515);
    }
    else if(landingType == Landing::Fall)
    {
        if(distance < getKPoint() + getKAndRealHSDifference() * 0.5)
            return (distance - getKPoint()) / ((getKAndRealHSDifference() / 1.1) / 0.01);
        else if(distance <= getRealHS())
            return (distance - getKPoint()) / ((getKAndRealHSDifference() / 1.1) / 0.018);
        else if(distance < getRealHS() * 1.12)
            return (distance - getRealHS()) / ((getKAndRealHSDifference() / 1.1) / 40);
        else
            return (distance - getRealHS()) / ((getKAndRealHSDifference() / 1.1) / 515);
    }
    return 0;
}

double Hill::getLandingImbalanceChangeByHillProfile(double distance)
{
    /// czym wi??ksza warto???? zwracana, tym teorytycznie mniejsze zachwianie przy l??dowaniu (a raczej mniejsza szansa na wy??sze zachwianie)
    if(distance <= getKPoint())
        return 0.07;
    else if(distance < getKPoint() + getKAndRealHSDifference() * 0.5)
        return 0.06;
    else if(distance <= getRealHS())
        return 0.05;
    else if(distance <= getRealHS() * 1.12)
        return 0.04;
    else
        return 0.03;
}

void Hill::updateCountryFlagPixmap(const QString & countryCode)
{
    setFlagPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(countryCode)));
}

QString Hill::getName() const
{
    return name;
}

void Hill::setName(const QString &newName)
{
    name = newName;
}

QString Hill::getCountryCode() const
{
    return countryCode;
}

void Hill::setCountryCode(const QString &newCountryCode)
{
    countryCode = newCountryCode;
    updateCountryFlagPixmap(newCountryCode);
}

double Hill::getKPoint() const
{
    return KPoint;
}

void Hill::setKPoint(double newKPoint)
{
    KPoint = newKPoint;
}

double Hill::getHSPoint() const
{
    return HSPoint;
}

void Hill::setHSPoint(double newHSPoint)
{
    HSPoint = newHSPoint;
}

double Hill::getPointsForMeter() const
{
    return pointsForMeter;
}

double Hill::getPointsForBackWind() const
{
    return pointsForBackWind;
}

void Hill::setPointsForBackWind(double newPointsForBackWind)
{
    pointsForBackWind = newPointsForBackWind;
}

double Hill::getPointsForGate() const
{
    return pointsForGate;
}

void Hill::setPointsForGate(double newPointsForGate)
{
    pointsForGate = newPointsForGate;
}

QSet<Characteristic> Hill::getCharacteristics() const
{
    return characteristics;
}

void Hill::setCharacteristics(const QSet<Characteristic> &newCharacteristics)
{
    characteristics = newCharacteristics;
}

double Hill::getPointsForFrontWind() const
{
    return pointsForFrontWind;
}

void Hill::setPointsForFrontWind(double newPointsForFrontWind)
{
    pointsForFrontWind = newPointsForFrontWind;
}
