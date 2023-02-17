#include "Hill.h"
#include "Landing.h"

#include <QDebug>


Hill::Hill(const QString &name, const QString &country, int KPoint, int HSPoint, double pointsForFrontWind, double pointsForGate, double takeoffEffect, double flightEffect, const QSet<Characteristic> &characteristics) : name(name),
    country(country),
    KPoint(KPoint),
    HSPoint(HSPoint),
    pointsForFrontWind(pointsForFrontWind),
    pointsForGate(pointsForGate),
    takeoffEffect(takeoffEffect),
    flightEffect(flightEffect)
{
    calculatePointsForBackWind();
    setCharacteristics(characteristics);
    setRealHSByCharacteristic();
    setupPointsForKPoint();
    setupPointsForMeter();
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

void Hill::setupPointsForMeter()
{
    if(KPoint <= 24)
        pointsForMeter = 4.8;
    else if(KPoint <= 29)
        pointsForMeter = 4.4;
    else if(KPoint <= 34)
        pointsForMeter = 4.0;
    else if(KPoint <= 39)
        pointsForMeter = 3.6;
    else if(KPoint <= 49)
        pointsForMeter = 3.2;
    else if(KPoint <= 59)
        pointsForMeter = 2.8;
    else if(KPoint <= 69)
        pointsForMeter = 2.4;
    else if(KPoint <= 79)
        pointsForMeter = 2.2;
    else if(KPoint <= 99)
        pointsForMeter = 2.0;
    else if(KPoint <= 169)
        pointsForMeter = 1.8;
    else if(KPoint >= 170)
        pointsForMeter = 1.2;
    else pointsForMeter = 0;
}

void Hill::setupPointsForKPoint()
{
    if(KPoint < 170)
        pointsForKPoint = 60;
    else pointsForKPoint = 120;
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
    /// czym większa wartość zwracana, tym teorytycznie mniejsze zachwianie przy lądowaniu (a raczej mniejsza szansa na wyższe zachwianie)
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

QString Hill::getName() const
{
    return name;
}

void Hill::setName(const QString &newName)
{
    name = newName;
}

QString Hill::getCountry() const
{
    return country;
}

void Hill::setCountry(const QString &newCountry)
{
    country = newCountry;
}

double Hill::getRecordDistance() const
{
    return recordDistance;
}

void Hill::setRecordDistance(double newRecordDistance)
{
    recordDistance = newRecordDistance;
}

int Hill::getKPoint() const
{
    return KPoint;
}

void Hill::setKPoint(int newKPoint)
{
    KPoint = newKPoint;
}

int Hill::getHSPoint() const
{
    return HSPoint;
}

void Hill::setHSPoint(int newHSPoint)
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

void Hill::calculatePointsForBackWind()
{
    pointsForBackWind = pointsForFrontWind * 1.21;
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
