#include "Hill.h"

#include <QDebug>


Hill::Hill(const QString &name, const QString &country, int KPoint, int HSPoint, double pointsForFrontWind, double pointsForGate, double landingZoneStart, double takeoffEffect, double flightEffect, const QSet<Characteristic> &characteristics) : name(name),
    country(country),
    KPoint(KPoint),
    HSPoint(HSPoint),
    pointsForFrontWind(pointsForFrontWind),
    pointsForGate(pointsForGate),
    landingZoneStart(landingZoneStart),
    takeoffEffect(takeoffEffect),
    flightEffect(flightEffect)
{
    calculatePointsForBackWind();
    setCharacteristics(characteristics);
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

double Hill::getLandingZoneStart() const
{
    return landingZoneStart;
}

void Hill::setLandingZoneStart(double newLandingZoneStart)
{
    landingZoneStart = newLandingZoneStart;
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
