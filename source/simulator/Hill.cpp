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
    autoPointsForBackWind(autoPointsForBackWind),
    ClassWithID()
{
    setCharacteristics(characteristics);
    setRealHSByCharacteristic();
}

QJsonObject Hill::getJsonObject(const Hill & hill)
{
    QJsonObject object;
    object.insert("id", QString::number(hill.getID()));
    object.insert("name", hill.getName());
    object.insert("country-code", hill.getCountryCode());
    object.insert("k-point", hill.getKPoint());
    object.insert("hs-point", hill.getHSPoint());

    if(hill.getAutoPointsForKPoint() == true)
        object.insert("points-for-meter", "auto");
    else object.insert("points-for-meter", hill.getPointsForMeter());

    if(hill.getAutoPointsForKPoint() == true)
        object.insert("points-for-k-point", "auto");
    else object.insert("points-for-k-point", hill.getPointsForKPoint());

    if(hill.getAutoPointsForBackWind() == true)
        object.insert("points-for-back-wind", "auto");
    else object.insert("points-for-back-wind", hill.getPointsForBackWind());

    object.insert("points-for-front-wind", hill.getPointsForFrontWind());
    object.insert("points-for-gate", hill.getPointsForGate());
    object.insert("takeoff-effect", hill.getTakeoffEffect());
    object.insert("flight-effect", hill.getFlightEffect());

    QJsonArray characteristicsArray;
    QSet<Characteristic> cs = hill.getCharacteristics();
    cs.detach();
    for(QSet<Characteristic>::iterator it = cs.begin(); it != cs.end(); ++it)
    {
        QJsonObject characteristicObject;
        characteristicObject.insert("type", it->getType());
        characteristicObject.insert("level", it->getLevel());
        characteristicsArray.push_back(characteristicObject);
    }
    object.insert("characteristics", characteristicsArray);

    return object;
}

Hill Hill::getFromJson(QJsonObject obj)
{
    Hill hill;
    hill.setID(obj.value("id").toString().toULong());
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
        hill.setPointsForBackWind(Hill::calculatePointsForBackWindBy50PercentsOfFrontWind(hill.getPointsForFrontWind()));
        hill.setAutoPointsForBackWind(true);
    }
    else hill.setPointsForBackWind(obj.value("points-for-back-wind").toDouble());

    hill.setTakeoffEffect(obj.value("takeoff-effect").toDouble());
    hill.setFlightEffect(obj.value("flight-effect").toDouble());

    QJsonArray characteristicsArray = obj.value("characteristics").toArray();
    for(const auto & val : characteristicsArray){
        hill.insertCharacteristic(val.toObject().value("type").toString(), val.toObject().value("level").toDouble());
    }

    return hill;
}

QVector<Hill> Hill::getVectorFromJson(const QByteArray &bytes)
{
    QVector<Hill> hills;
    hills.clear();
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(bytes, &error);
    if(error.error != QJsonParseError::NoError)
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Błąd przy wczytytywaniu skoczni", "Nie udało się wczytać skoczni z pliku userData/GlobalDatabase/globalHills.json\nTreść błędu: " + error.errorString(), QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return hills;
    }

    QJsonObject object = document.object();
    QJsonValue value = object.value("hills");
    if(!value.isArray())
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Błąd przy wczytytywaniu skoczni", "Prawdopodobnie w tym pliku nie ma listy ze skoczniami\nUpewnij się, że wybrałeś właściwy plik!");
        message.setModal(true);
        message.exec();
        return hills;
    }
    QJsonArray array = value.toArray();
    for(const auto & val : array)
    {
        hills.push_back(Hill::getFromJson(val.toObject()));
    }

    return hills;
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

double Hill::calculatePointsForBackWindBy50PercentsOfFrontWind(double pointsForFrontWind)
{
    return pointsForFrontWind * 1.50;
}

double Hill::calculateBestTakeoffHeightLevel(Hill *hill)
{
    if(hill->getTakeoffEffect() > hill->getFlightEffect())
        return 1.85 + ((hill->getTakeoffEffect() / hill->getFlightEffect()) - 1) / 0.186;
    else
        return 1.85 + -((hill->getFlightEffect() / hill->getTakeoffEffect()) - 1) / 0.186;

    return 0;
}

double Hill::calculateBestFlightHeightLevel(Hill *hill)
{
    if(hill->getTakeoffEffect() > hill->getFlightEffect())
        return 2.1 + (hill->getTakeoffEffect() / hill->getFlightEffect() - 1) / 0.1425;
    else
        return 2.1 + -(hill->getFlightEffect() / hill->getTakeoffEffect() - 1) / 0.1425;

    return 0;
}

int Hill::getHillType()
{
    if(HSPoint < 50)
        return Hill::Small;
    else if(HSPoint < 85)
        return Hill::Average;
    else if(HSPoint < 110)
        return Hill::Normal;
    else if(HSPoint < 150)
        return Hill::Large;
    else if(HSPoint < 186)
        return Hill::Big;
    else
        return Hill::Flying;
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
    realHS *= 1 + (getLevelOfCharacteristic("real-hs-point") * 0.015);
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
            return (distance - getKPoint()) / ((getKAndRealHSDifference() / 1.1) / 10);
        else if(distance <= getRealHS())
            return (distance - getKPoint()) / ((getKAndRealHSDifference() / 1.1) / 12);
        else if (distance < getRealHS() * 1.12)
            return (distance - getRealHS()) / ((getKAndRealHSDifference() / 1.1) / 170);
        else
            return (distance - getRealHS()) / ((getKAndRealHSDifference() / 1.1) / 280);
    }
    else if(landingType == Landing::BothLegsLanding)
    {
        if(distance < getKPoint() + getKAndRealHSDifference() * 0.5)
            return (distance - getKPoint()) / ((getKAndRealHSDifference() / 1.1) / 2);
        else if(distance <= getRealHS())
            return (distance - getKPoint()) / ((getKAndRealHSDifference() / 1.1) / 4.8);
        else if (distance < getRealHS() * 1.12)
            return (distance - getRealHS()) / ((getKAndRealHSDifference() / 1.1) / 280);
        else
            return (distance - getRealHS()) / ((getKAndRealHSDifference() / 1.1) / 400);
    }
    else if(landingType == Landing::SupportLanding)
    {
        if(distance < getKPoint() + getKAndRealHSDifference() * 0.5)
            return (distance - getKPoint()) / ((getKAndRealHSDifference() / 1.1) / 0.008);
        else if(distance <= getRealHS())
            return (distance - getKPoint()) / ((getKAndRealHSDifference() / 1.1) / 0.02);
        else if (distance < getRealHS() * 1.12)
            return (distance - getRealHS()) / ((getKAndRealHSDifference() / 1.1) / 61);
        else
            return (distance - getRealHS()) / ((getKAndRealHSDifference() / 1.1) / 600);
    }
    else if(landingType == Landing::Fall)
    {
        if(distance < getKPoint() + getKAndRealHSDifference() * 0.5)
            return (distance - getKPoint()) / ((getKAndRealHSDifference() / 1.1) / 0.01);
        else if(distance <= getRealHS())
            return (distance - getKPoint()) / ((getKAndRealHSDifference() / 1.1) / 0.018);
        else if(distance < getRealHS() * 1.12)
            return (distance - getRealHS()) / ((getKAndRealHSDifference() / 1.1) / 70);
        else
            return (distance - getRealHS()) / ((getKAndRealHSDifference() / 1.1) / 1800);
    }
    return 0;
}

double Hill::getLandingImbalanceChangeByHillProfile(double distance)
{
    /// czym większa wartość zwracana, tym teorytycznie mniejsze zachwianie przy lądowaniu (a raczej mniejsza szansa na wyższe zachwianie)
    if(distance <= getKPoint())
        return 0.01;
    else if(distance < getKPoint() + getKAndRealHSDifference() * 0.5)
        return 0.05;
    else if(distance <= getRealHS())
        return 0.1;
    else if(distance <= getRealHS() * 1.12)
        return 0.2;
    else
        return 0.4;
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
